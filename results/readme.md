Serial Plotter View:
The cyan waveform represents the ECG signal, showing each heartbeat as a rhythmic pulse.
The yellow BPM text displays the heart rate value, updating dynamically with every detected peak.
The red stress text highlights the stress classification — in this example, HIGH, signaling elevated heart activity.
This visual makes the heartbeat tangible, turning electrical impulses into an intuitive graph of human emotion.

Serial Monitor View:
The window titled COM3 (Arduino Uno) confirms the device is actively streaming data.
The first lines — “ECG Monitor (PRO)”, “BPM: 105”, and “Stress: HIGH” — summarize the current physiological state.
A stream of numeric data like “520,105”, “530,105”, etc., represents filtered ECG values and BPM readings used for plotting.

When stress crosses the threshold, the system sends an alert:
“ALERT! High Stress BPM: 108”, notifying that the user’s stress level has spiked.
