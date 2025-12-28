# HSL-Timetable
A small terminal application for checking bus, tram, metro, railway, and ferry timetables for Helsinki Regional Transport (HSL).

## Installation
### From source
Clone the repositroy
```
git clone https://github.com/oskarTom/HSL_timetable
```
Assuming that `gcc` is installed, the source can be compiled with `Makefile`
```
cd HSL_timetable
make
```
## Configuration
### API
First, get your API key by following instructions from [here](https://digitransit.fi/en/developers/api-registration/).
TThen set your API key in `$HOME/.config/hsltt/hsltt.conf` file

```
api-key = YOUR_API_KEY
```
### Default stop
Optionally, you can set a default stop which will be queried when hsltt is ran without parameters
```
default-stop = STOP_NAME
```
