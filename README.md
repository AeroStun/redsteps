# RedSteps
_Temperature gamma offset a-la `xgamma`_

There is both a command line tool `redsteps` and an optional GUI `redsteps-gtk`.  
Both currently use the XF86VideoMode X11 extension. The GUI additionally uses gtkmm-3.0.

### Building

```shell
cmake -DBUILD_GUI=Yes -B dist
cmake --build dist
```

### Running

Usage: `redsteps <temperature (in Kelvin, min 1000, neutral 6500, max 25000)>`

