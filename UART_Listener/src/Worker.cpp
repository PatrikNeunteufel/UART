/**
 ****************************************************************************************
 * @file   Worker.cpp
 * @brief  Worker thread implementations for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#include "Worker.hpp"
#include "Time.hpp"
#include "Globals.hpp"

#include <conio.h>

namespace uart_listener
{

    void serialReaderThread(HANDLE hSerial, Channel channel)
    {
        constexpr size_t     kBufferSize = 512;
        std::vector<uint8_t> buffer(kBufferSize);

        // Create overlapped event for this thread
        HANDLE hReadEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        if (hReadEvent == NULL)
        {
            std::cerr << "Failed to create read event\n";
            return;
        }

        OVERLAPPED ov{};
        ov.hEvent = hReadEvent;

        HANDLE waitHandles[2] = { g_hStopEvent, hReadEvent };

        while (!g_stopRequested.load())
        {
            ResetEvent(hReadEvent);

            DWORD bytesRead = 0;
            BOOL  ok = ReadFile(
                hSerial,
                buffer.data(),
                static_cast<DWORD>(buffer.size()),
                &bytesRead,
                &ov);

            if (!ok)
            {
                DWORD err = GetLastError();
                if (err == ERROR_IO_PENDING)
                {
                    // Wait for either data or stop signal
                    DWORD waitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

                    if (waitResult == WAIT_OBJECT_0)
                    {
                        // Stop event signaled
                        CancelIo(hSerial);
                        break;
                    }
                    else if (waitResult == WAIT_OBJECT_0 + 1)
                    {
                        // Read completed
                        if (!GetOverlappedResult(hSerial, &ov, &bytesRead, FALSE))
                        {
                            DWORD ovErr = GetLastError();
                            if (ovErr != ERROR_OPERATION_ABORTED)
                            {
                                std::cerr << "\nOverlapped read error (code: " << ovErr << ")\n";
                            }
                            break;
                        }
                    }
                    else
                    {
                        // Wait failed
                        break;
                    }
                }
                else
                {
                    std::cerr << "\nSerial read error (code: " << err << ")\n";
                    g_stopRequested.store(true);
                    SetEvent(g_hStopEvent);
                    g_packetQueue.notifyStop();
                    break;
                }
            }

            if (bytesRead > 0)
            {
                Packet pkt;
                pkt.channel = channel;
                pkt.timestamp = getTimestampWithMs();
                pkt.data.assign(buffer.begin(), buffer.begin() + bytesRead);
                g_packetQueue.push(std::move(pkt));
            }
        }

        CloseHandle(hReadEvent);
    }

    void keyboardMonitorThread()
    {
        // Ensure console input is in the right mode for _kbhit/_getch
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn != INVALID_HANDLE_VALUE)
        {
            // Flush any pending input from interactive prompts
            FlushConsoleInputBuffer(hIn);
        }

        while (!g_stopRequested.load())
        {
            // Method 1: Use _kbhit/_getch (conio.h)
            if (_kbhit())
            {
                int key = _getch();

                // Handle extended keys (arrows, function keys return 0 or 0xE0 first)
                if (key == 0 || key == 0xE0)
                {
                    (void)_getch(); // Consume the second byte
                    continue;
                }

                if (key == 27) // ESC
                {
                    std::cout << "\n[ESC pressed]\n" << std::flush;
                    g_stopRequested.store(true);
                    SetEvent(g_hStopEvent);
                    g_packetQueue.notifyStop();
                    break;
                }

                // Also allow 'q' or 'Q' to quit
                if (key == 'q' || key == 'Q')
                {
                    std::cout << "\n[Q pressed]\n" << std::flush;
                    g_stopRequested.store(true);
                    SetEvent(g_hStopEvent);
                    g_packetQueue.notifyStop();
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    // Alternative: Use Windows Console API directly for more reliable key detection
    void keyboardMonitorThreadWinAPI()
    {
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Warning: Cannot get console input handle\n";
            return;
        }

        // Save original console mode
        DWORD originalMode = 0;
        GetConsoleMode(hIn, &originalMode);

        // Set console mode for raw input
        SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT);

        // Flush any pending input
        FlushConsoleInputBuffer(hIn);

        INPUT_RECORD inputRecord;
        DWORD        eventsRead;

        while (!g_stopRequested.load())
        {
            // Check if input is available (non-blocking)
            DWORD numEvents = 0;
            if (!GetNumberOfConsoleInputEvents(hIn, &numEvents) || numEvents == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            if (ReadConsoleInputA(hIn, &inputRecord, 1, &eventsRead) && eventsRead > 0)
            {
                if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
                {
                    WORD vkCode = inputRecord.Event.KeyEvent.wVirtualKeyCode;
                    char ch = inputRecord.Event.KeyEvent.uChar.AsciiChar;

                    // ESC key (VK_ESCAPE = 0x1B)
                    if (vkCode == VK_ESCAPE || ch == 27)
                    {
                        std::cout << "\n[ESC pressed]\n" << std::flush;
                        g_stopRequested.store(true);
                        SetEvent(g_hStopEvent);
                        g_packetQueue.notifyStop();
                        break;
                    }

                    // Also allow 'q' or 'Q'
                    if (ch == 'q' || ch == 'Q')
                    {
                        std::cout << "\n[Q pressed]\n" << std::flush;
                        g_stopRequested.store(true);
                        SetEvent(g_hStopEvent);
                        g_packetQueue.notifyStop();
                        break;
                    }
                }
            }
        }

        // Restore original console mode
        SetConsoleMode(hIn, originalMode);
    }
}