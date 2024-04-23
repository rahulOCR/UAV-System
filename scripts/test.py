import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter

def butter_lowpass_filter(data, cutoff_frequency, sampling_rate, order=4):
    nyquist = 0.5 * sampling_rate
    normal_cutoff = cutoff_frequency / nyquist
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = lfilter(b, a, data)
    return y

# Replace 'your_file.csv' with the actual path to your CSV file
csv_file_path = 'data.csv'

# Read CSV file into a pandas DataFrame
df = pd.read_csv(csv_file_path)

# Access the column named 'PL.pX'
pl_px_column = df['PL.pY']

# Apply low-pass filter
sampling_rate = 1000  # Replace with your actual sampling rate
cutoff_frequency = 50  # Replace with your desired cutoff frequency
filtered_pl_px_column = butter_lowpass_filter(pl_px_column, cutoff_frequency, sampling_rate)

# Plot original and filtered signals
plt.figure(figsize=(10, 6))

plt.subplot(2, 1, 1)
plt.plot(pl_px_column, label='RAW POS-X')
plt.title('RAW POS-X')
#plt.xlabel('Index')
#plt.ylabel('Amplitude')
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(filtered_pl_px_column, label=f'Low-pass Filtered (cutoff={cutoff_frequency} Hz)')
plt.title('Low-pass Filtered Pos-X')
#plt.xlabel('Index')
#plt.ylabel('Amplitude')
plt.legend()

plt.tight_layout()
plt.show()

