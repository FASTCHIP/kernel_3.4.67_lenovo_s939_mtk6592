#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov8825qtechraw.h"
#include "camera_info_ov8825qtechraw.h"
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
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
        67160,    // i4R_AVG
        18335,    // i4R_STD
        103460,    // i4B_AVG
        28328,    // i4B_STD
        {  // i4P00[9]
            4392000, -1390000, -440000, -696000, 3166000, 88000, 82000, -2552000, 5028000
        },
        {  // i4P10[9]
            888562, -786151, -106296, -20283, -55518, 66801, 21382, 691243, -698175
        },
        {  // i4P01[9]
            418154, -400510, -19945, -112133, -170822, 275009, -45265, -396573, 450386
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
            1144,    // u4MinGain, 1024 base = 1x
            10240,    // u4MaxGain, 16x
            100,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            26,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            18,    // u4VideoExpUnit  
            31,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            28,    // u4CapExpUnit 
            15,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 200, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {75, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,    // bEnableCaptureThres
            TRUE,    // bEnableVideoThres
            FALSE,    // bEnableStrobeThres
            47,    // u4AETarget
            0,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -5,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            0,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            50    // u4FlatnessStrength
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
                842,    // i4R
                512,    // i4G
                658    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                91,    // i4X
                -276    // i4Y
            },
            // Horizon
            {
                -425,    // i4X
                -305    // i4Y
            },
            // A
            {
                -309,    // i4X
                -317    // i4Y
            },
            // TL84
            {
                -155,    // i4X
                -354    // i4Y
            },
            // CWF
            {
                -103,    // i4X
                -444    // i4Y
            },
            // DNP
            {
                15,    // i4X
                -341    // i4Y
            },
            // D65
            {
                91,    // i4X
                -276    // i4Y
            },
            // DF
            {
                65,    // i4X
                -385    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                62,    // i4X
                -285    // i4Y
            },
            // Horizon
            {
                -456,    // i4X
                -259    // i4Y
            },
            // A
            {
                -341,    // i4X
                -283    // i4Y
            },
            // TL84
            {
                -192,    // i4X
                -336    // i4Y
            },
            // CWF
            {
                -149,    // i4X
                -431    // i4Y
            },
            // DNP
            {
                -21,    // i4X
                -341    // i4Y
            },
            // D65
            {
                62,    // i4X
                -285    // i4Y
            },
            // DF
            {
                24,    // i4X
                -390    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                842,    // i4R
                512,    // i4G
                658    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                603,    // i4G
                1618    // i4B
            },
            // A 
            {
                518,    // i4R
                512,    // i4G
                1195    // i4B
            },
            // TL84 
            {
                671,    // i4R
                512,    // i4G
                1020    // i4B
            },
            // CWF 
            {
                813,    // i4R
                512,    // i4G
                1073    // i4B
            },
            // DNP 
            {
                829,    // i4R
                512,    // i4G
                796    // i4B
            },
            // D65 
            {
                842,    // i4R
                512,    // i4G
                658    // i4B
            },
            // DF 
            {
                942,    // i4R
                512,    // i4G
                790    // i4B
            }
        },
        // Rotation matrix parameter
        {
            6,    // i4RotationAngle
            255,    // i4Cos
            27    // i4Sin
        },
        // Daylight locus parameter
        {
            -155,    // i4SlopeNumerator
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
            -242,    // i4RightBound
            -892,    // i4LeftBound
            -221,    // i4UpperBound
            -321    // i4LowerBound
            },
            // Warm fluorescent
            {
            -242,    // i4RightBound
            -892,    // i4LeftBound
            -321,    // i4UpperBound
            -441    // i4LowerBound
            },
            // Fluorescent
            {
            -71,    // i4RightBound
            -242,    // i4LeftBound
            -213,    // i4UpperBound
            -383    // i4LowerBound
            },
            // CWF
            {
            -71,    // i4RightBound
            -242,    // i4LeftBound
            -383,    // i4UpperBound
            -481    // i4LowerBound
            },
            // Daylight
            {
            137,    // i4RightBound
            -71,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Shade
            {
            447,    // i4RightBound
            137,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            137,    // i4RightBound
            -71,    // i4LeftBound
            -366,    // i4UpperBound
            -481    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            447,    // i4RightBound
            -892,    // i4LeftBound
            0,    // i4UpperBound
            -481    // i4LowerBound
            },
            // Daylight
            {
            162,    // i4RightBound
            -71,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Cloudy daylight
            {
            262,    // i4RightBound
            87,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Shade
            {
            362,    // i4RightBound
            87,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Twilight
            {
            -71,    // i4RightBound
            -231,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
            },
            // Fluorescent
            {
            112,    // i4RightBound
            -292,    // i4LeftBound
            -235,    // i4UpperBound
            -481    // i4LowerBound
            },
            // Warm fluorescent
            {
            -241,    // i4RightBound
            -441,    // i4LeftBound
            -235,    // i4UpperBound
            -481    // i4LowerBound
            },
            // Incandescent
            {
            -241,    // i4RightBound
            -441,    // i4LeftBound
            -205,    // i4UpperBound
            -366    // i4LowerBound
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
            827,    // i4R
            512,    // i4G
            674    // i4B
            },
            // Cloudy daylight
            {
            965,    // i4R
            512,    // i4G
            557    // i4B
            },
            // Shade
            {
            1025,    // i4R
            512,    // i4G
            517    // i4B
            },
            // Twilight
            {
            653,    // i4R
            512,    // i4G
            903    // i4B
            },
            // Fluorescent
            {
            782,    // i4R
            512,    // i4G
            900    // i4B
            },
            // Warm fluorescent
            {
            579,    // i4R
            512,    // i4G
            1307    // i4B
            },
            // Incandescent
            {
            520,    // i4R
            512,    // i4G
            1198    // i4B
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
            6424    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5164    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            2027    // i4OffsetThr
            },
            // Daylight WB gain
            {
            805,    // i4R
            512,    // i4G
            683    // i4B
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
                -518,    // i4RotatedXCoordinate[0]
                -403,    // i4RotatedXCoordinate[1]
                -254,    // i4RotatedXCoordinate[2]
                -83,    // i4RotatedXCoordinate[3]
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
}};  //  NSFeature


