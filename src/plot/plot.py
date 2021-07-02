import matplotlib.pyplot as plt
import pandas as pd

excel = pd.read_excel("log.xlsx")
plt.plot(excel["Time"], excel["Indoor Temperature"], color="green")
plt.savefig("inTemp.png")
plt.close()

plt.plot(excel["Time"], excel["Outdoor Temperature"], color="green", label="Outdoor Temperature")
plt.savefig("outTemp.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor Humidity"], color="green", label="Indoor Humidity")
plt.savefig("inHum.png")
plt.close()

plt.plot(excel["Time"], excel["Outdoor Humidity"], color="green", label="Outdoor Humidity")
plt.savefig("outHum.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor CO2"], color="green", label="Indoor CO2")
plt.savefig("inCO2.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor Pressure"], color="green", label="Indoor Pressure")
plt.savefig("inPres.png")
plt.close()

plt.plot(excel["Time"], excel["Outdoor Pressure"], color="green", label="Outdoor Pressure")
plt.savefig("outPres.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor PM2.5"], color="green", label="Indoor PM2.5")
plt.savefig("inPM25.png")
plt.close()

plt.plot(excel["Time"], excel["Outdoor PM2.5"], color="green", label="Outdoor PM2.5")
plt.savefig("outPM25.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor PM10"], color="green", label="Indoor PM10")
plt.savefig("inPM10.png")
plt.close()

plt.plot(excel["Time"], excel["Outdoor PM10"], color="green", label="Outdoor PM10")
plt.savefig("outPM10.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor TVOC"], color="green", label="Indoor TVOC")
plt.savefig("inTVOC.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor Temperature"], color="green", label="Indoor Temperature")
plt.plot(excel["Time"], excel["Outdoor Temperature"], color="red", label="Outdoor Temperature")
plt.savefig("temp.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor Humidity"], color="green", label="Indoor Humidity")
plt.plot(excel["Time"], excel["Outdoor Humidity"], color="red", label="Outdoor Humidity")
plt.savefig("hum.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor Pressure"], color="green", label="Indoor Pressure")
plt.plot(excel["Time"], excel["Outdoor Pressure"], color="red", label="Outdoor Pressure")
plt.savefig("pres.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor PM2.5"], color="green", label="Indoor PM2.5")
plt.plot(excel["Time"], excel["Outdoor PM2.5"], color="red", label="Outdoor PM2.5")
plt.savefig("PM25.png")
plt.close()

plt.plot(excel["Time"], excel["Indoor PM10"], color="green", label="Indoor PM10")
plt.plot(excel["Time"], excel["Outdoor PM10"], color="red", label="Outdoor PM10")
plt.savefig("PM10.png")
plt.close()

