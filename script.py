import numpy as np
import matplotlib.pyplot as plt
import fourier_transform
def low_pass_filter(s, w):
    return w / (s + w)

t_low = 0
t_high = 0.5
dt = 0.0001
period = 0.1
magnitude = 0.15
freq = 2000
cutoff_freq = 1000

# Fourier
fourier_result, frequencies = fourier_transform.sft(period, magnitude, freq, t_low, t_high, dt)

# low-pass filter
filtered_fourier = fourier_result * np.array([low_pass_filter(f, cutoff_freq) for f in frequencies])

# inverse Fourier
filtered_signal = fourier_transform.inverse_sft(filtered_fourier, frequencies, t_low, t_high, dt)

# the original wave
t_values = np.arange(t_low, t_high, dt)
square_wave_signal = np.where(t_values < period, 1, 0) + magnitude * np.sin(2 * np.pi * freq * t_values)

plt.figure(figsize=(12, 6))

# original vs filtered signals
plt.subplot(2, 1, 1)
plt.plot(t_values, square_wave_signal, label="Original Signal")
plt.plot(t_values, filtered_signal[:len(t_values)], label="Filtered Signal", color='orange')
plt.title('Square Wave - Original vs Filtered')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend()

# original vs filtered spectrums
plt.subplot(2, 1, 2)
plt.plot(frequencies[:len(fourier_result) // 2], np.abs(fourier_result  )[:len(fourier_result) // 2], label='Original Fourier')
plt.plot(frequencies[:len(fourier_result) // 2], np.abs(filtered_fourier)[:len(fourier_result) // 2], label='Filtered Fourier', color='orange')
plt.title('Fourier Transform - Original vs Filtered')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.legend()

plt.tight_layout()
plt.show()
