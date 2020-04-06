peertube-viewer
===

a command line program to view browse peertube, inspired by the youtube-viewer utility


Dependencies
---
- readline
- rapidjson
- curl

Compiling
---
```bash
make
```

Installing
---
```bash
make install
```


Usage
---
Here is an example of basic usage :

The user wants to use the peertube instance video.ploud.fr. They search for videos about mastodon then they select the 13th search result. Additional information about the video is displayed and the video is launched. If no player has been chosen as argument and no player is selected in the config file, it will default to mpv to play the video. When the video is ended, the user can search other videos, play another search result or quit with :q.
![Screenshot of basic usage](docs/screens/peertube-viewer.png?raw=true "Exemple usage")

To see all available options see:
```bash
peertube-viewer -h
```
or read the manpage

Contributing
===

If you have a feature idea and want to implement it you are welcome.
If you want to contribute, do not hesitate to take a look at the feature that will be added in [TODO.md](TODO.md)
The master branch is the latest released version, while the development happens on the dev branch.

The UI and the logic are separated, with the goal that maybe someday, there will be a GUI interface and a CLI one.

The build uses GCC to automatically create the .hpp dependencies of each .cpp file.

Unit tests
------------
The program contains unit tests in src/tests.

To run them:
```bash
make test
```
Note: this requires having installed the catch2 or catch libraries.
If catch is installed installed instead of catch2, the right command will be:
```bash
make test-catch-v1
```


