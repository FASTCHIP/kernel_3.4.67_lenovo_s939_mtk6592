#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov8865mipiraw.h"
#include "camera_info_ov8865mipiraw.h"
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
        68150,    // i4R_AVG
        16330,    // i4R_STD
        76975,    // i4B_AVG
        18680,    // i4B_STD
        {  // i4P00[9]
            4952500, -2255000, -137500, -932500, 3807500, -312500, -87500, -2687500, 5332500
        },
        {  // i4P10[9]
            1028050, -1064259, 36208, 24087, -253037, 226157, 125267, 548347, -670821
        },
        {  // i4P01[9]
            884449, -938033, 53584, -172335, -331445, 504666, -36434, -433590, 469138
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
            8192,    // u4MaxGain, 16x
            80,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            27,    // u4PreExpUnit 
            31,    // u4PreMaxFrameRate
            18,    // u4VideoExpUnit  
            31,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            14,    // u4CapExpUnit 
            30,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            372    // u4FocusLength_100x
        },
        // rHistConfig
        {
            4,    // u4HistHighThres
            30,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            170,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 230, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 100, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 18}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,    // bEnableCaptureThres
            TRUE,    // bEnableVideoThres
            TRUE,    // bEnableStrobeThres
            45,    // u4AETarget
            0,    // u4StrobeAETarget
            50,    // u4InitIndex
            5,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            4,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            70,    // u4InDoorEV = 9.0, 10 base 
            -9,    // i4BVOffset delta BV = value/10 
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
                843,    // i4R
                512,    // i4G
                542    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                0,    // i4X
                0    // i4Y
            },
            // Horizon
            {
                -369,    // i4X
                -196    // i4Y
            },
            // A
            {
                -259,    // i4X
                -219    // i4Y
            },
            // TL84
            {
                -108,    // i4X
                -232    // i4Y
            },
            // CWF
            {
                -52,    // i4X
                -316    // i4Y
            },
            // DNP
            {
                69,    // i4X
                -276    // i4Y
            },
            // D65
            {
                163,    // i4X
                -205    // i4Y
            },
            // DF
            {
                128,    // i4X
                -296    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                0,    // i4X
                0    // i4Y
            },
            // Horizon
            {
                -372,    // i4X
                -190    // i4Y
            },
            // A
            {
                -262,    // i4X
                -215    // i4Y
            },
            // TL84
            {
                -112,    // i4X
                -230    // i4Y
            },
            // CWF
            {
                -57,    // i4X
                -315    // i4Y
            },
            // DNP
            {
                65,    // i4X
                -277    // i4Y
            },
            // D65
            {
                160,    // i4X
                -208    // i4Y
            },
            // DF
            {
                123,    // i4X
                -298    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                647,    // i4G
                1389    // i4B
            },
            // A 
            {
                512,    // i4R
                541,    // i4G
                1033    // i4B
            },
            // TL84 
            {
                606,    // i4R
                512,    // i4G
                812    // i4B
            },
            // CWF 
            {
                732,    // i4R
                512,    // i4G
                842    // i4B
            },
            // DNP 
            {
                817,    // i4R
                512,    // i4G
                678    // i4B
            },
            // D65 
            {
                843,    // i4R
                512,    // i4G
                542    // i4B
            },
            // DF 
            {
                909,    // i4R
                512,    // i4G
                642    // i4B
            }
        },
        // Rotation matrix parameter
        {
            1,    // i4RotationAngle
            256,    // i4Cos
            4    // i4Sin
        },
        // Daylight locus parameter
        {
            -132,    // i4SlopeNumerator
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
            -162,    // i4RightBound
            -812,    // i4LeftBound
            -152,    // i4UpperBound
            -252    // i4LowerBound
            },
            // Warm fluorescent
            {
            -162,    // i4RightBound
            -812,    // i4LeftBound
            -252,    // i4UpperBound
            -372    // i4LowerBound
            },
            // Fluorescent
            {
            15,    // i4RightBound
            -162,    // i4LeftBound
            -140,    // i4UpperBound
            -272    // i4LowerBound
            },
            // CWF
            {
            15,    // i4RightBound
            -162,    // i4LeftBound
            -272,    // i4UpperBound
            -365    // i4LowerBound
            },
            // Daylight
            {
            215,    // i4RightBound
            15,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
            },
            // Shade
            {
            545,    // i4RightBound
            215,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            215,    // i4RightBound
            15,    // i4LeftBound
            -302,    // i4UpperBound
            -402    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            545,    // i4RightBound
            -812,    // i4LeftBound
            0,    // i4UpperBound
            -402    // i4LowerBound
            },
            // Daylight
            {
            240,    // i4RightBound
            15,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
            },
            // Cloudy daylight
            {
            340,    // i4RightBound
            165,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
            },
            // Shade
            {
            440,    // i4RightBound
            165,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
            },
            // Twilight
            {
            15,    // i4RightBound
            -145,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
            },
            // Fluorescent
            {
            210,    // i4RightBound
            -212,    // i4LeftBound
            -158,    // i4UpperBound
            -365    // i4LowerBound
            },
            // Warm fluorescent
            {
            -162,    // i4RightBound
            -362,    // i4LeftBound
            -158,    // i4UpperBound
            -365    // i4LowerBound
            },
            // Incandescent
            {
            -162,    // i4RightBound
            -362,    // i4LeftBound
            -128,    // i4UpperBound
            -302    // i4LowerBound
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
            815,    // i4R
            512,    // i4G
            572    // i4B
            },
            // Cloudy daylight
            {
            963,    // i4R
            512,    // i4G
            482    // i4B
            },
            // Shade
            {
            1030,    // i4R
            512,    // i4G
            450    // i4B
            },
            // Twilight
            {
            631,    // i4R
            512,    // i4G
            746    // i4B
            },
            // Fluorescent
            {
            732,    // i4R
            512,    // i4G
            726    // i4B
            },
            // Warm fluorescent
            {
            517,    // i4R
            512,    // i4G
            1040    // i4B
            },
            // Incandescent
            {
            485,    // i4R
            512,    // i4G
            977    // i4B
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
            6658    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5387    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1749    // i4OffsetThr
            },
            // Daylight WB gain
            {
            743,    // i4R
            512,    // i4G
            618    // i4B
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
                -532,    // i4RotatedXCoordinate[0]
                -422,    // i4RotatedXCoordinate[1]
                -272,    // i4RotatedXCoordinate[2]
                -95,    // i4RotatedXCoordinate[3]
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


