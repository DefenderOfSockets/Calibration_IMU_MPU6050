import csv
from os import makedirs
from itertools import islice
from scipy.spatial.transform import Rotation
import pandas as pd
from matplotlib import pyplot as plt
import allantools


class DataProcessingCalibration:
    def __init__(self):
        self.header = []
        self.dirSave = 'C://PlotsFromTest/'

        makedirs(self.dirSave, exist_ok=True)

    def openFile(self, dataFile):
        # Write header of data file in list
        with open(dataFile, 'rt') as f:
            readerCSV = csv.reader(f)
            for line in islice(readerCSV, 4):
                self.header.append(line)

            # Write data from the .csv in the dataframe
            df = pd.read_csv(dataFile, header=4)

        return df

    def processFile(self, df):
        # Convert quaternion in Euler angles (Roll, Pitch, Yaw)
        dfEuler = self.quatToEuler(df)

        return dfEuler

    def plotDataFromFile(self, dfEuler, tau_roll, ad_roll, tau_pitch, ad_pitch, tau_yaw, ad_yaw):
        fig, ax = plt.subplots(6, figsize=(10, 26))
        fig.suptitle("Plots of parameters from static test\n" + str(self.header[0][0].split(':')[1]),
                     fontsize=12, y=0.9)

        ax[0].plot(dfEuler['time'], dfEuler['roll'])
        ax[0].set(xlabel='Time, sec', ylabel='Roll, degrees')

        ax[1].plot(dfEuler['time'], dfEuler['pitch'])
        ax[1].set(xlabel='Time, sec', ylabel='Pitch, degrees')

        ax[2].plot(dfEuler['time'], dfEuler['yaw'])
        ax[2].set(xlabel='Time, sec', ylabel='Yaw, degrees')

        ax[3].plot(tau_pitch, ad_roll)
        ax[3].set(xscale='log', yscale='log', xlabel='Time, sec', ylabel='Allan deviation of Roll')

        ax[4].plot(tau_pitch, ad_pitch)
        ax[4].set(xscale='log', yscale='log', xlabel='Time, sec', ylabel='Allan deviation of Pitch')

        ax[5].plot(tau_yaw, ad_yaw)
        ax[5].set(xscale='log', yscale='log', xlabel='Time, sec\n\n\n' +
            'Max values (Roll, Pitch, Yaw): ' + str(round(dfEuler['roll'].max(), 3)) + "  " + str(round(dfEuler['pitch'].max(), 3)) + "  " + str(round(dfEuler['yaw'].max(), 3)) + '\n\n' +
            'Min values (Roll, Pitch, Yaw): ' + str(round(dfEuler['roll'].min(), 3)) + "  " + str(round(dfEuler['pitch'].min(), 3)) + "  " + str(round(dfEuler['yaw'].min(), 3)) + '\n\n' +
            'Mean values (Roll, Pitch, Yaw): ' + str(round(dfEuler['roll'].mean(), 3)) + "  " + str(round(dfEuler['pitch'].mean(), 3)) + "  " + str(round(dfEuler['yaw'].mean(), 3)) + '\n\n',
            ylabel='Allan deviation of Yaw')

        for i in range(len(ax)):
            ax[i].grid()

        fig.savefig(self.dirSave + 'PlotsFromStaticTest - ' + str(self.header[0][0].split(':')[1]) + '.png', dpi=300)

        self.header.clear()

    def quatToEuler(self, df):
        quat, roll, pitch, yaw, time = [], [], [], [], []

        for i in range(len(df['time_save'])):
            quat.append([df['x'][i], df['y'][i], df['z'][i], df['w'][i]])

            rotation = Rotation.from_quat(quat)
            rotation = rotation.as_euler('xyz', degrees=True)

            roll.append(rotation[0][0])
            pitch.append(rotation[0][1])
            yaw.append(rotation[0][2])
            time.append(df['time_save'][i])

            quat.clear()

        return pd.DataFrame({'time': time, 'roll': roll, 'pitch': pitch, 'yaw': yaw})

    def deviationAllan(self, dfEuler, rate):
        data_roll = list(dfEuler['roll'])
        data_pitch = list(dfEuler['pitch'])
        data_yaw = list(dfEuler['yaw'])

        (tau_roll, ad_roll, ade, adn) = allantools.oadev(data_roll, rate=rate)
        (tau_pitch, ad_pitch, ade, adn) = allantools.oadev(data_pitch, rate=rate)
        (tau_yaw, ad_yaw, ade, adn) = allantools.oadev(data_yaw, rate=rate)

        return tau_roll, ad_roll, tau_pitch, ad_pitch, tau_yaw, ad_yaw
