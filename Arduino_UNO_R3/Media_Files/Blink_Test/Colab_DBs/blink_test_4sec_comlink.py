# -*- coding: utf-8 -*-
"""Blink_Test_4sec_comLink.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1kYlDORHyYn4ePcZFzhgTsZ3kCWznCDi2

# This is a script to plot and generate a report for current analysis in a microcontroller development board kit.
Author: Alexandre Nuernberg

Date: 2024/04/13

# MCU: Arduino Uno R3

# Sketch: Blink 1 second
"""



"""Things to Note:

CSV Structure: I've assumed your CSV data has the correct headers. If not, adjust the column names.

Timestamp Format: If your timestamp has a different format, modify the format parameter in the pd.to_datetime function accordingly.

"""

# Mounting your Google Drive
from google.colab import drive
drive.mount('/content/drive')

# Checking drive content:
# !ls -l "/content/drive/MyDrive/Colab_DBs/"

# Loading the CSV File
data_file = "/content/drive/MyDrive/Colab_DBs/Blink_Test_4sec.csv"

# Create the DataFrame with the csv file downloaded
df = pd.read_csv(data_file)

import os
if os.path.exists(data_file):
    print("File exists!")
else:
    print("File does not exist.")

# If needed Unmount the Google Drive
#drive.flush_and_unmount() #comment if you not use

# You can use this method if you do not want to share your all drive folder.
# It is safer.

# Just try this method if everything is working fine with the full mount of GD which is simpler to test

# Link for de DB file: Blink_Test_4sec.csv
#file_url = 'https://drive.google.com/uc?export=download&id=1_P9UPbpq97x9oa-tUArgX6Vqqchez0Vf'

# Request to download the db file
#response = requests.get(file_url)

# Create the DataFrame with the csv file downloaded
#df = pd.read_csv(io.StringIO(response.content.decode('utf-8')))

# df = pd.read_csv(data_file) # just check if the file is there

# Importing Libraries
import matplotlib.pyplot as plt
import pandas as pd
import requests
import io
import locale

# Configure para usar o ponto como separador decimal (por exemplo, padrão dos EUA)
locale.setlocale(locale.LC_NUMERIC, 'en_US.UTF-8')



# Splitting the "Timestamp" Column
# Assuming timestamps are in milliseconds
df['Timestamp(ms)'] = df['Timestamp(ms)'].astype(int)  # Convert to integers

# Since we only have milliseconds, let's create a simulated 'Date' column
# You can customize the start date if needed
df['Date'] = pd.to_datetime('2024-04-09')

# Convert timestamps from ms to datetime objects for x-axis
df['Time'] = pd.to_datetime(df['Timestamp(ms)'], unit='ms')

print(df.head())

"""# Converting the current to (mA)"""

df['Current(mA)'] = df['Current(uA)'] / 1000

print(df.head())

# Creating the Histogram

# Converte vírgulas para pontos
#df['Current(mA)'] = [str(val).replace(',', '.') for val in df['Current(mA)']]

# Crie o histograma
# Criar objeto de figura:
fig, ax = plt.subplots(figsize=(12, 6))  # Adjust figure size (width, height)

#Gerar histograma:
ax.hist(df['Current(uA)'], bins=50)

#Personalizar (títulos, labels, etc):
ax.set_xlabel('Timestamp (ms)')
ax.set_ylabel('Current (uA)')
ax.set_title('Arduino UNO R3 Power Consumption for Blink Sketch')
ax.grid(True)

#Salvar a figura:
fig.savefig('/content/drive/MyDrive/Colab_DBs/histogram.png', dpi=900)  # Save with 900 DPI

# Creating the Boxplot

fig, ax = plt.subplots(figsize=(8, 5))  # Cria a figura e o subplot
plt.boxplot(df['Current(uA)'])
plt.ylabel('Current (uA)')
plt.title('Boxplot of Arduino UNO R3 Power Consumption')
plt.grid(True)

fig.savefig('/content/drive/MyDrive/Colab_DBs/boxplot.png', dpi=900)  # Salva a figura
plt.show()  # Exibe a figura

# Creating the Line Chart

fig, ax = plt.subplots(figsize=(10,6))  # Cria a figura e o subplot
#plt.figure(figsize=(10, 6))  # Adjust figure size as needed
plt.plot(df['Timestamp(ms)'], df['Current(uA)'])
plt.xlabel('Timestamp (ms)')
plt.ylabel('Current (uA)')
plt.title('Line Chart of Arduino UNO R3 Power Consumption')
plt.grid(True)
fig.savefig('/content/drive/MyDrive/Colab_DBs/linechart.png', dpi=900)  # Save with 900 DPI
plt.show()

"""# Calculating mean and standard deviation"""

# Calculating Mean and Standard Deviation

mean_current = df['Current(uA)'].mean()
std_current = df['Current(uA)'].std()
max_current = df['Current(uA)'].max()
min_current = df['Current(uA)'].min()

# Convert to milliamperes
mean_current_mA = mean_current / 1000
std_current_mA = std_current / 1000
max_current_mA = max_current / 1000
min_current_mA = min_current / 1000

# Format with 2 decimal places and units
print("Mean Current: {:.2f} mA".format(mean_current_mA))
print("Standard Deviation of Current: {:.2f} mA".format(std_current_mA))
print("Maximum Current: {:.2f} mA".format(max_current_mA))
print("Minimum Current: {:.2f} mA".format(min_current_mA))

# Calculate the interval time of the analysis (assuming milliseconds, change if needed)
start_time = df['Timestamp(ms)'].iloc[0] / 1000
end_time = df['Timestamp(ms)'].iloc[-1] / 1000
interval_time = end_time - start_time
print("Interval Time of the Analysis: {:.2f} sec".format(interval_time ))

# ... seu código existente ...

# Caminho para o arquivo
file_path = '/content/drive/MyDrive/Colab_DBs/data.txt'

# Cria a string com os dados formatados
data_content = (
    f"Mean Current: {mean_current_mA:.2f} mA\n"
    f"Standard Deviation of Current: {std_current_mA:.2f} mA\n"
    f"Maximum Current: {max_current_mA:.2f} mA\n"
    f"Minimum Current: {min_current_mA:.2f} mA\n"
    f"Interval Time of the Analysis: {interval_time:.2f} sec\n"
)

# Escreve os dados no arquivo (modo 'w' cria ou sobreescreve o arquivo)
with open(file_path, 'w') as f:
    f.write(data_content)

print("Data saved to", file_path)

"""# Criando o Relatório em PDF:

# criando um template latex no drive com nome: relatorio.tex


"""

# Install the fpdf module
!pip install fpdf

"""#Gere o PDF com FPDF

"""

# ... seu código Python existente ...

# Import the FPDF library
from fpdf import FPDF

# Create the PDF
pdf = FPDF()
pdf.add_page()
pdf.set_font('Arial', size=12)

# Read the LaTeX content from the file
with open('/content/drive/MyDrive/Colab_DBs/relatorio.tex', 'r') as f:
    latex_content = f.read()

    # Replace the placeholders with the calculated values
    latex_content = latex_content.replace('[MEAN_CURRENT]', str(mean_current_mA))
    latex_content = latex_content.replace('[STD_CURRENT]', str(std_current_mA))
    # ... outras substituições

# Write the LaTeX content to the PDF
pdf.write(5, latex_content)

# Save the PDF
pdf.output('/content/drive/MyDrive/Colab_DBs/relatorio_Arduino.pdf')