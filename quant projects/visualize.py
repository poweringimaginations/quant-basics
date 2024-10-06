# make sure to run python3 visualize.py if you're on an older version

import pandas as pd
import matplotlib.pyplot as plt

# Load the data from CSV
data = pd.read_csv('output.csv')

# Plot Closing Price and 10-Day SMA
plt.figure(figsize=(10, 6))
plt.plot(data['Date'], data['Close Price'], label='Close Price')
plt.plot(data['Date'], data['SMA'], label='10-Day SMA', linestyle='--')
plt.xticks(rotation=45)
plt.title('Stock Prices and 10-Day SMA')
plt.legend()

# Plot RSI
plt.figure(figsize=(10, 6))
plt.plot(data['Date'], data['RSI'], label='RSI', color='green')
plt.axhline(70, color='red', linestyle='--', label='Overbought (70)')
plt.axhline(30, color='blue', linestyle='--', label='Oversold (30)')
plt.xticks(rotation=45)
plt.title('RSI')
plt.legend()

# Plot MACD Line, Signal Line, and Histogram
plt.figure(figsize=(10, 6))
plt.plot(data['Date'], data['MACD Line'], label='MACD Line')
plt.plot(data['Date'], data['Signal Line'], label='Signal Line', linestyle='--')
plt.bar(data['Date'], data['MACD Histogram'], label='MACD Histogram', color='gray', alpha=0.5)
plt.xticks(rotation=45)
plt.title('MACD')
plt.legend()

# Show all the plots together at the end
plt.show()
