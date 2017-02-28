# Changlog

Changes to Leapfrog-Firmware 

## 2.6

- Changed bed volume and homing position to make sure that actual buildvolume is ok
- Fixed critical bug on software endstop declaration

| Buildvolume | Xmin | Xmax | Ymin | Ymax | Zmin | Zmax 
|---| --- | --- | --- | --- | --- | --- 
| Old | `0.0` | `367.0` | `0.0` | `355.0` | `0.0` | `205.0` 
| **New** | `0.0` | `367.0` | **`-33.0`** | **`322.0`** | `0.0` | `205.0`

## 2.5

- Fixed temperature reporting. 2.4 had an output error
- Changed max feedrate of Z-axis due to steps max
- Changed bed width calibration to negative output in M115

###Config changes
| Name | Old | New
|---|---|---
DEFAULT_MAX_FEEDRATE | `400, 400, 40, 200` | `400, 400, 20, 200`

## 2.4

- Changed temperature reporting
Removed the active extruder temperature.
OLD: `T: 20 / 0 B: 20 / 0 T0: 20 / 0 T1: 21 /0 @: 0`
NEW: `T0: 20 / 0 T1: 21 /0 B: 20 / 0  @: 0`
This is done because Mirror mode makes "active extruder" always be T1 that makes LUI act weird.

###Config changes
| Name | Old | New
|---|---|---
| DEFAULT_MAX_ACCELERATION | `450, 450, 100, 30` | `400,400,100,30`
| DEFAULT_ACCELERATION | `1000` | `400`
| DEFAULT_XYJERK | `25` | `20`


## Beta

- Dual X Calibration M219
- RTD sensor support
- No movements for M605
- G28 is now allowed (and should be required) after M605
- Fixed toolchange during sync mode
- Updated M115 with bed width calibration
- Updated acceleration/velocity settings Setting 

###Config changes
| Name | Old | New
|---|---|---
DEFAULT_MAX_FEEDRATE | `400, 400, 40, 400` | `400, 400, 40, 200`
DEFAULT_MAX_ACCELERATION | `1250,1250,100,10000` | `450,450,100,30` 
DEFAULT_RETRACT_ACCELERATION | `5000` | `1250`
DEFAULT_XYJERK | `15.0` | `25.0`