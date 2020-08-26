#!/usr/bin/python3
from fmiopendata.wfs import download_stored_query

obsQuery = "fmi::observations::weather::cities::multipointcoverage"
fore = "fmi::forecast::hirlam::surface::obsstations::multipointcoverage"



class Observations:
    obsQuery = "fmi::observations::weather::cities::multipointcoverage"
    query =  download_stored_query(obsQuery)
    locations = sorted(query.location_metadata.keys())
    times = sorted(query.data.keys())
    
    def latestReading(self, location):
        i = -1
        while location not in self.query.data[self.times[i]].keys():
            i = i - 1

        return self.query.data[self.times[i]][location], self.times[i]
    
    def allReadings(self, location):
        result = {} 
        for time in self.times:
            if location in self.query.data[time].keys():
                result[time] = self.query.data[time][location]

        return result


if __name__ == "__main__":
    obs = Observations()
    #print(obs.locations)
    # reading, time = obs.latestReading("Tampere Tampella")
    readings = obs.allReadings("Jokioinen Ilmala") 
    for time in readings.keys():
        for key in readings[time].keys():
            print(key, ":", readings[time][key]["value"])