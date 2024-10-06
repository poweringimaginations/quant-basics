#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// store stock data from csv file
struct StockData {
    string date;
    double open;
    double high;
    double low;
    double close;
};

// nasdaq formatting is scuffed and uses '$'
// helper function removes dollar sign to import data correctly
double removeDollarSign(const string& str) {
    string numStr = str;
    numStr.erase(remove(numStr.begin(), numStr.end(), '$'), numStr.end()); 
    return stod(numStr);  // double conversion
}

// read CSV file
vector<StockData> readCSV(const string& filename) {
    ifstream file(filename);
    vector<StockData> data;
    string line, word;
    
    // skip header
    getline(file, line);
    
    // Read each line of the CSV
    while (getline(file, line)) {
        stringstream ss(line);
        StockData stock;
        
        // parsing stocks
        getline(ss, stock.date, ',');
        string open, high, low, close;
        getline(ss, open, ',');
        getline(ss, word, ',');  // skip stock volume
        getline(ss, high, ',');
        getline(ss, low, ',');
        getline(ss, close, ',');
        
        // remove dollar sign
        stock.open = removeDollarSign(open);
        stock.high = removeDollarSign(high);
        stock.low = removeDollarSign(low);
        stock.close = removeDollarSign(close);

        data.push_back(stock);
    }

    file.close();
    return data;
}

// 10 day Simple Moving Average
// SMA calculates the average closing prices of AAPL over the past 10 days. 
// each day starting from the tenth day,
// sum the closing prices of the previous 10 days and divide by 10
// you can compare the current stock price to the SMA to decide whether to buy or sell

double calculateMovingAverage(const vector<StockData>& data, int period, size_t currentIndex) {
    double sum = 0;
    for (size_t i = currentIndex - period + 1; i <= currentIndex; i++) {
        sum += data[i].close;
    }
    return sum / period;
}

// 2) RSI (Relative Strength Index)
// identifies whether a stock is overbrought or oversold using its gains/losses over a given period
// RSI = 100 - ((100)/(1+(averageGain/averageLoss)))

double calculateRSI(const vector<StockData>& data, int period, size_t currentIndex) {
    double gain = 0, loss = 0;
    for (size_t i = currentIndex - period + 1; i <= currentIndex; i++) {
        double change = data[i].close - data[i - 1].close;
        if (change > 0) {
            gain += change;
        } else {
            loss -= change;
        }
    }
    double averageGain = gain / period;
    double averageLoss = loss / period;

    if (averageLoss == 0) return 100;  // Avoid division by zero

    double rs = averageGain / averageLoss;
    return 100 - (100 / (1 + rs));
}

// 3) MACD (moving average convergence divergence)
double calculateEMA(const vector<StockData>& data, int period, size_t currentIndex, double previousEMA) {
    double k = 2.0 / (period + 1);  // Smoothing factor
    return data[currentIndex].close * k + previousEMA * (1 - k);
}


int main() {
    string filename = "appleSixMonth.csv"; 
    vector<StockData> stockData = readCSV(filename);

    // check if data loaded
    if (!stockData.empty()) {
        cout << "Data loaded successfully! First entry:\n";
        cout << "Date: " << stockData[0].date << ", Close Price: " << stockData[0].close << endl;
    } else {
        cout << "Failed to load data.\n";
        return 1;
    }

    int period = 10;

    // initialize exponential moving average values for MACD calculation
    double ema12 = stockData[11].close;  // EMA12
    double ema26 = stockData[25].close;  // EMA26 
    double signalLine = 0;  // set as 0 for now, will calculate later

    // create output file for visualizing results
    ofstream outFile("output.csv");
    outFile << "Date,Close Price,SMA,RSI,MACD Line,Signal Line,MACD Histogram\n";  // headers

    // calculate the 10 day SMA, RSI, and MACD for every entry in the CSV file
    for (size_t i = period - 1; i < stockData.size(); i++) {
        // 10 day SMA
        double movingAverage = calculateMovingAverage(stockData, period, i);
        cout << "Date: " << stockData[i].date << ", 10-Day Moving Average: " << movingAverage << endl;

        // 14 day RSI
        double rsi = 0;
        if (i >= 14) { 
            rsi = calculateRSI(stockData, 14, i);
            cout << "Date: " << stockData[i].date << ", RSI: " << rsi << endl;
        }

        // MACD calculation
        double macdLine = 0, macdHistogram = 0;
        if (i >= 25) { // starts after the 26th day (need at least 26 data points)
            ema12 = calculateEMA(stockData, 12, i, ema12);
            ema26 = calculateEMA(stockData, 26, i, ema26);
            macdLine = ema12 - ema26;

            // calculate signal line (9-period EMA of MACD line) after at least 9 MACD values
            if (i >= 25 + 9) {
                signalLine = calculateEMA(stockData, 9, i, signalLine);
                macdHistogram = macdLine - signalLine;
                cout << "Date: " << stockData[i].date << ", MACD Line: " << macdLine 
                     << ", Signal Line: " << signalLine << ", MACD Histogram: " << macdHistogram << endl;
            }
        }

        // transfer values to new CSV file
        outFile << stockData[i].date << "," << stockData[i].close << "," << movingAverage << "," 
                << rsi << "," << macdLine << "," << signalLine << "," << macdHistogram << "\n";
    }

    // closes output file
    outFile.close();

    return 0;
}



