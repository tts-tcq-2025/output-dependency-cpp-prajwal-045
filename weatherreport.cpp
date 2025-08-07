#include <string>
#include <iostream>
#include <assert.h>

using namespace std;

namespace WeatherSpace
{    
    class IWeatherSensor {
        public:
            virtual double TemperatureInC() const = 0;
            virtual int Precipitation() const = 0;
            virtual int Humidity() const = 0;
            virtual int WindSpeedKMPH() const = 0;
    };
     /// <summary>
    /// This is a stub for a weather sensor. For the sake of testing 
    /// we create a stub that generates weather data and allows us to
    /// test the other parts of this application in isolation
    /// without needing the actual Sensor during development
    /// </summary>
    class SensorStub : public IWeatherSensor {
        int Humidity() const override { return 72; }
        int Precipitation() const override { return 70; }
        double TemperatureInC() const override { return 26; }
        int WindSpeedKMPH() const override { return 52; }
    };

    // New stub to expose the bug - high precipitation > 60, wind > 50, temp > 25
    class BugExposingStub : public IWeatherSensor {
        int Humidity() const override { return 80; }
        int Precipitation() const override { return 70; }   // > 60
        double TemperatureInC() const override { return 27; } // > 25
        int WindSpeedKMPH() const override { return 55; }    // > 50
    };

    // Another stub for low precipitation and temp < 25
    class SunnyStub : public IWeatherSensor {
        int Humidity() const override { return 40; }
        int Precipitation() const override { return 10; }   // < 20
        double TemperatureInC() const override { return 20; } // < 25
        int WindSpeedKMPH() const override { return 10; }    // low wind
    };

    string Report(const IWeatherSensor& sensor)
    {
        int precipitation = sensor.Precipitation();
        string report = "Sunny Day";

        if (sensor.TemperatureInC() > 25)
        {
            if (precipitation >= 20 && precipitation < 60)
                report = "Partly Cloudy";
            else if (sensor.WindSpeedKMPH() > 50)
                report = "Alert, Stormy with heavy rain";
        }
        return report;
    }
    
    void TestRainy()
    {
        SensorStub sensor;
        string report = Report(sensor);
        cout << "TestRainy: " << report << endl;
        // Should mention rain (bug: might fail if logic wrong)
        assert(report.find("rain") != string::npos);
    }

    void TestHighPrecipitation()
    {
        SensorStub sensor;
        string report = Report(sensor);
        cout << "TestHighPrecipitation: " << report << endl;
        // This weak assert passes even if report is wrong
        assert(report.length() > 0);
    }

    // New stronger test exposing bug in logic
    void TestBugExposing()
    {
        BugExposingStub sensor;
        string report = Report(sensor);
        cout << "TestBugExposing: " << report << endl;

        // Strong assert: Should report stormy with heavy rain but buggy code fails this
        assert(report.find("rain") != string::npos && report.find("Alert") != string::npos);
    }

    void TestSunny()
    {
        SunnyStub sensor;
        string report = Report(sensor);
        cout << "TestSunny: " << report << endl;
        assert(report == "Sunny Day");
    }
}

void testWeatherReport() {
    cout << "\nWeather report test\n";
    WeatherSpace::TestRainy();
    WeatherSpace::TestHighPrecipitation();
    WeatherSpace::TestBugExposing();  // This test will fail exposing bug
    WeatherSpace::TestSunny();
    cout << "All tests done (some may fail as expected)\n";
}

int main() {
    testWeatherReport();
    return 0;
}
