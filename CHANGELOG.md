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