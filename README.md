# Simple Logical Tautology Handler (SLoTH)

SLoTH is a simple model that assumes something is true because you say it is. This could be defining an output to be a constant value, or that an output value is equal to some other value that SLoTH receives as input.

Practically, it exists to support scenarios where multiple BMI model modules are combined to into a single model to achieve capabilities not natively supported by a framework. For instance, if you are using a model that requires a `soil_ice_fraction` input, but you are using that model in sub-saharan Africa, you can easily use SLoTH to provide a constant value of `0` for that input instead of running a more complex model to provide the same value.

Other use-cases exist, for example in [ngen](https://github.com/NOAA-OWP/ngen) you could use SLoTH at the *end* of a Multi-BMI formulation to echo a `temperature` forcing value as `temperature_tminus1`, which ngen would then make available at the *beginning* of the next timestep of the formulation. In fact, the new output could be proxied again, and this could be repeated multiple times to echo multiple previous timestep values (not presently supported by ngen directly) and used to feed a simple linear regression model. Other use cases might include fixing values in a complex formulation for integration test or calibration purposes.

## Dependencies

Sloth has no dependencies on other libraries, but includes the [bmi-cxx](https://github.com/csdms/bmi-cxx) header file repository and [googletest](https://github.com/google/googletest/) as Git submodules, which will be automatically downloaded into the code tree during installation.

## Installation

See [INSTALL](INSTALL.md).

## Configuration / Usage

At present, SLoTH does not read any configuration file (an empty string should be provided to `Initialize()`, and it will be ignored--though this feature is expected to be added!). Instead, configuration of tautologies is done by setting values--any variable and value set on the model will become a new output variable of SLoTH. 

``` c++
auto s = new Sloth();
double v = 0.0;
s.SetValue("adouble", &v);
// ^ Creates a new, single, type double output variable named `adouble` that will always be 0.
```

This is especially well suited to take advantage of [ngen's `model_param` mechanism](https://github.com/NOAA-OWP/ngen/pull/368), as you can create SLoTH variables directly in the realization config; variables set using `model_params` as if they were configuration settings will become output variables with the set value.

By default, any newly set value will be assumed to have the following metadata properties:

| Property | Value
|----------|--------
| Count | `1`
| Type | `"double"`
| Units | `"1"` (unit, dimensionless)
| Location | `"node"`

It is also possible, however, to specify these metadata properties by appending a set of parameters in parentheses to the variable name. For example:

``` c++
auto s = new Sloth();
double[3] somedoubles = { 42.0, 43.0, 44.0 };
s.SetValue("somedoubles(3)", &somedoubles);
// ^ Creates an array of 3 doubles with units "1"
int[4] someints = { 1, 1, 3, 8 };
s.SetValue("someints(4,int,cm)", &someints);
// ^ Creates an array of 4 ints with units of centimeters.
float[3] someedgyfloats = { 45.0, 46.0, 47.0 };
s.SetValue("someedgyfloats(3,float,m,edge)", &someedgyfloats);
// ^ Creates an array of 3 floats wiht units of meters with grid location "edge"
```

> NOTE: At present, you cannot have whitespace around parameters!

As shown above, you do not have to specify all the parameters, however none can be omitted (e.g. if you want to specify units, you will have to specify the count and type to get there).

Lastly, it is possible to specify an *input alias* as a final parameter:

``` c++
auto s = new Sloth();
double v = 0.0;
s.SetValue("smellssweet(1,double,1,node,arose)", &v);
// ^ Creates an output variable `smellssweet` that always reports the same value as last input to variable `arose`, with all metadata properties the same as the defaults.
```

You can (as may be apparent) call `SetValue(...)` on any SLoTH output variable at any time to change its output value, but setting an input alias as above causes the alias to appear in the output of `GetInputVarNames()` and thus be recognized by a BMI framework as an input variable (the metadata properties are of course also reported for the input alias). Note that you can set up *multiple* output variables with the *same* input alias--in which case any received input value will be replicated to all applicable outputs--which may be useful in some scenarios.

Importantly, the metadata parameters do not have to be part of the variable every time it is set, only the first time.

``` c++
auto s = new Sloth();
int[4] someints = { 1, 1, 3, 8 };
s.SetValue("someints(4,int,cm)", &someints);
// ^ Initializes the `someints` output variable with the specified properties
someints[2] = 2; someints[3] = 3;
s.SetValue("someints", &someints);
// ^ Changes the output 
```

Also, getting values via the metadata-parameterized name is not supported, only the bare name should be used.

``` c++
auto s = new Sloth();
int[4] someints = { 1, 1, 3, 8 };
s.SetValue("someints(4,int,cm)", &someints);
// ^ 👍
s.GetValue("someints(4,int,cm)", &someints);
// ^ Will probably fail
```
> NOTE: You may be able to change some property values such as units by calling `SetValue(...)` with metadata property parameters again, but this is not supported and will likely fail if changing type or count, for instance.

Finally, note that it is not implied that any conversions take place within SLoTH based on the provided metadata. Conversions of units or datatypes may be done by an applied framework based on the metadata values returned, but SLoTH does none of this work!

## How to test the software

See [INSTALL.md](INSTALL.md)

## Known issues

* Grid metadata BMI functions (e.g. `GetGridShape(...)`) are not implemented!
* Since time functions (such as `Update()`) do nothing of consequence, some unexpected results may occur. The timestep for the "model" is actually `std::numeric_limits<double>::lowest()`... so if a framework tries to call `Update()` repeatedly until an expected time is reported by `GetCurrentTime()`...well...that may take a while. It is assumed that frameworks will call `UpdateUntil(...)` instead.
  
## Getting help

If you have questions, concerns, bug reports, etc, please file an issue in this repository's Issue Tracker.
## Getting involved

Please feel free to submit PRs! Especially for the following improvements:

* Config file reading (ideally support JSON or YAML? But with what dependencies?)
* Grid metadata methods (This will probably require config file support)

----

## Open source licensing info
1. [TERMS](TERMS.md)
2. [LICENSE](LICENSE)


----

## Credits and references

1. [Next Generation Water Resources Modelling Framework](https://github.com/NOAA-OWP/ngen)