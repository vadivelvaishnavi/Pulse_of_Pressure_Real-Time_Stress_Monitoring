Signal Acquisition:
ECG signals are read from A0.
Safety pins (LO+, LO-) check if electrodes are properly connected.

Signal Filtering:
A simple moving average filter smooths the raw ECG signal to reduce noise.

Peak Detection & BPM Calculation:
Peaks above a threshold indicate heartbeats.
Time between beats → converted into BPM (beats per minute).
A buffer averages BPM values for stability.

Stress Classification:
LOW stress → BPM < 60 (Green LED).
MEDIUM stress → BPM 60–100 (Yellow LED).
HIGH stress → BPM > 100 (Red LED + SMS alert).

Display & Feedback:
OLED shows BPM and Stress level.
LEDs give quick visual feedback.
Serial plotter outputs ECG waveform + BPM for debugging.

Alert System:
If stress is classified as HIGH, an SMS alert is sent with BPM details.


