import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier, plot_tree
from sklearn.metrics import classification_report, confusion_matrix
import matplotlib.pyplot as plt

# access data from output.csv (make sure to load quantBasics.cpp first)
data = pd.read_csv('output.csv')

# create target column to see if price went up or down
data['Target'] = (data['Close Price'].shift(-1) > data['Close Price']).astype(int)
data = data[:-1]  # Remove the last row

# using SMA, RSI, MACD line, Signal Line, MACD Histogram
features = data[['SMA', 'RSI', 'MACD Line', 'Signal Line', 'MACD Histogram']]

# training/testing
X_train, X_test, y_train, y_test = train_test_split(features, data['Target'], test_size=0.2, random_state=42)

# initialize decision tree classifier
clf = DecisionTreeClassifier()
clf.fit(X_train, y_train)

# predict
predictions = clf.predict(X_test)

# evaluate model
print(classification_report(y_test, predictions))
print(confusion_matrix(y_test, predictions))

# plot tree
plt.figure(figsize=(20,10))
plot_tree(clf, filled=True, feature_names=features.columns, class_names=["Down", "Up"])
plt.show()
