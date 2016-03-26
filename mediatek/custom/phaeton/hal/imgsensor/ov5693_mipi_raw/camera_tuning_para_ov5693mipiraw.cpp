#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov5693mipiraw.h"
#include "camera_info_ov5693mipiraw.h"
#include "camera_custom_AEPlinetable.h"
#include "camera_custom_tsf_tbl.h"


const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        66400,    // i4R_AVG
        16663,    // i4R_STD
        94350,    // i4B_AVG
        19562,    // i4B_STD
        {  // i4P00[9]
            4917500, -1957500, -400000, -642500, 3892500, -690000, -37500, -2275000, 4872500
        },
        {  // i4P10[9]
            1636237, -1639556, 3318, -175077, -94156, 269232, 42123, 342427, -371431
        },
        {  // i4P01[9]
            939483, -968218, 28735, -252484, -98902, 351386, -5003, -125228, 139108
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1152,    // u4MinGain, 1024 base = 1x
            10240,    // u4MaxGain, 16x
            51,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            18,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            18,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            18,    // u4CapExpUnit 
            30,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            22,    // u4LensFno, Fno = 2.8 //zhangjiano modify for cts
            338    // u4FocusLength_100x  //zhangjiano modify for cts
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            30,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            170,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,            // bEnableCaptureThres
            TRUE,            // bEnableVideoThres
            TRUE,            // bEnableStrobeThres
            47,    // u4AETarget
            0,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            4,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            1,    // i4BVOffset delta BV = value/10 
            4,    // u4PreviewFlareOffset
            4,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            4,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            2,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            55    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                793,    // i4R
                512,    // i4G
                719    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                36,    // i4X
                -287    // i4Y
            },
            // Horizon
            {
                -514,    // i4X
                -212    // i4Y
            },
            // A
            {
                -385,    // i4X
                -244    // i4Y
            },
            // TL84
            {
                -190,    // i4X
                -304    // i4Y
            },
            // CWF
            {
                -147,    // i4X
                -383    // i4Y
            },
            // DNP
            {
                -33,    // i4X
                -329    // i4Y
            },
            // D65
            {
                36,    // i4X
                -287    // i4Y
            },
            // DF
            {
                6,    // i4X
                -302    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                36,    // i4X
                -287    // i4Y
            },
            // Horizon
            {
                -514,    // i4X
                -212    // i4Y
            },
            // A
            {
                -385,    // i4X
                -244    // i4Y
            },
            // TL84
            {
                -190,    // i4X
                -304    // i4Y
            },
            // CWF
            {
                -147,    // i4X
                -383    // i4Y
            },
            // DNP
            {
                -33,    // i4X
                -329    // i4Y
            },
            // D65
            {
                36,    // i4X
                -287    // i4Y
            },
            // DF
            {
                6,    // i4X
                -302    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                793,    // i4R
                512,    // i4G
                719    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                771,    // i4G
                2057    // i4B
            },
            // A 
            {
                512,    // i4R
                620,    // i4G
                1454    // i4B
            },
            // TL84 
            {
                598,    // i4R
                512,    // i4G
                1000    // i4B
            },
            // CWF 
            {
                706,    // i4R
                512,    // i4G
                1049    // i4B
            },
            // DNP 
            {
                765,    // i4R
                512,    // i4G
                836    // i4B
            },
            // D65 
            {
                793,    // i4R
                512,    // i4G
                719    // i4B
            },
            // DF 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            }
        },
        // Rotation matrix parameter
        {
            0,    // i4RotationAngle
            256,    // i4Cos
            0    // i4Sin
        },
        // Daylight locus parameter
        {
            -103,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -240,    // i4RightBound
            -890,    // i4LeftBound
            -178,    // i4UpperBound
            -278    // i4LowerBound
            },
            // Warm fluorescent
            {
            -240,    // i4RightBound
            -890,    // i4LeftBound
            -278,    // i4UpperBound
            -398    // i4LowerBound
            },
            // Fluorescent
            {
            -83,    // i4RightBound
            -240,    // i4LeftBound
            -192,    // i4UpperBound
            -343    // i4LowerBound
            },
            // CWF
            {
            -83,    // i4RightBound
            -240,    // i4LeftBound
            -343,    // i4UpperBound
            -433    // i4LowerBound
            },
            // Daylight
            {
            61,    // i4RightBound
            -83,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Shade
            {
            421,    // i4RightBound
            61,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            61,    // i4RightBound
            -83,    // i4LeftBound
            -367,    // i4UpperBound
            -467    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            421,    // i4RightBound
            -890,    // i4LeftBound
            0,    // i4UpperBound
            -467    // i4LowerBound
            },
            // Daylight
            {
            86,    // i4RightBound
            -83,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Cloudy daylight
            {
            186,    // i4RightBound
            11,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Shade
            {
            286,    // i4RightBound
            11,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Twilight
            {
            -83,    // i4RightBound
            -243,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Fluorescent
            {
            86,    // i4RightBound
            -290,    // i4LeftBound
            -237,    // i4UpperBound
            -433    // i4LowerBound
            },
            // Warm fluorescent
            {
            -285,    // i4RightBound
            -485,    // i4LeftBound
            -237,    // i4UpperBound
            -433    // i4LowerBound
            },
            // Incandescent
            {
            -285,    // i4RightBound
            -485,    // i4LeftBound
            -207,    // i4UpperBound
            -367    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            757,    // i4R
            512,    // i4G
            754    // i4B
            },
            // Cloudy daylight
            {
            863,    // i4R
            512,    // i4G
            661    // i4B
            },
            // Shade
            {
            923,    // i4R
            512,    // i4G
            618    // i4B
            },
            // Twilight
            {
            606,    // i4R
            512,    // i4G
            942    // i4B
            },
            // Fluorescent
            {
            702,    // i4R
            512,    // i4G
            925    // i4B
            },
            // Warm fluorescent
            {
            478,    // i4R
            512,    // i4G
            1357    // i4B
            },
            // Incandescent
            {
            448,    // i4R
            512,    // i4G
            1272    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            7091    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            4780    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1342    // i4OffsetThr
            },
            // Daylight WB gain
            {
            750,    // i4R
            512,    // i4G
            765    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -550,    // i4RotatedXCoordinate[0]
                -421,    // i4RotatedXCoordinate[1]
                -226,    // i4RotatedXCoordinate[2]
                -69,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace

const CAMERA_TSF_TBL_STRUCT CAMERA_TSF_DEFAULT_VALUE =
{
    #include INCLUDE_FILENAME_TSF_PARA
    #include INCLUDE_FILENAME_TSF_DATA
};


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T),
                                             0,
                                             sizeof(CAMERA_TSF_TBL_STRUCT)};

    if (CameraDataType > CAMERA_DATA_TSF_TABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        case CAMERA_DATA_TSF_TABLE:
            memcpy(pDataBuf,&CAMERA_TSF_DEFAULT_VALUE,sizeof(CAMERA_TSF_TBL_STRUCT));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature


