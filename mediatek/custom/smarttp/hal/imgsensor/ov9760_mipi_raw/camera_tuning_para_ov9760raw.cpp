#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov9760raw.h"
#include "camera_info_ov9760raw.h"
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
        79675,    // i4R_AVG
        20263,    // i4R_STD
        102500,    // i4B_AVG
        26194,    // i4B_STD
        {  // i4P00[9]
            4742500, -1787500, -392500, -632500, 3487500, -297500, -27500, -2012500, 4602500
        },
        {  // i4P10[9]
            964576, -1001034, 32078, -108118, -24507, 137006, 101739, 601619, -704322
        },
        {  // i4P01[9]
            620476, -638953, 14926, -252563, -126441, 382555, -692, 23606, -28384
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
            1136,    // u4MinGain, 1024 base = 1x
            16384,    // u4MaxGain, 16x
            140,    // u4MiniISOGain, ISOxx  
            32,    // u4GainStepUnit, 1x/8 
            28934,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            28934,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            28934,    // u4CapExpUnit 
            30,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            235    // zhangjiano modify u4FocusLength_100x  
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
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
            TRUE,    // bEnableStrobeThres
            42,    // u4AETarget
            0,    // u4StrobeAETarget
            45,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            0,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            0,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            64,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            58    // u4FlatnessStrength
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
                976,    // i4R
                512,    // i4G
                735    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                105,    // i4X
                -372    // i4Y
            },
            // Horizon
            {
                -490,    // i4X
                -360    // i4Y
            },
            // A
            {
                -354,    // i4X
                -384    // i4Y
            },
            // TL84
            {
                -167,    // i4X
                -445    // i4Y
            },
            // CWF
            {
                -118,    // i4X
                -539    // i4Y
            },
            // DNP
            {
                13,    // i4X
                -433    // i4Y
            },
            // D65
            {
                105,    // i4X
                -372    // i4Y
            },
            // DF
            {
                75,    // i4X
                -483    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                92,    // i4X
                -376    // i4Y
            },
            // Horizon
            {
                -503,    // i4X
                -343    // i4Y
            },
            // A
            {
                -367,    // i4X
                -372    // i4Y
            },
            // TL84
            {
                -183,    // i4X
                -439    // i4Y
            },
            // CWF
            {
                -137,    // i4X
                -535    // i4Y
            },
            // DNP
            {
                -2,    // i4X
                -433    // i4Y
            },
            // D65
            {
                92,    // i4X
                -376    // i4Y
            },
            // DF
            {
                58,    // i4X
                -486    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                976,    // i4R
                512,    // i4G
                735    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                611,    // i4G
                1931    // i4B
            },
            // A 
            {
                533,    // i4R
                512,    // i4G
                1390    // i4B
            },
            // TL84 
            {
                747,    // i4R
                512,    // i4G
                1172    // i4B
            },
            // CWF 
            {
                905,    // i4R
                512,    // i4G
                1246    // i4B
            },
            // DNP 
            {
                937,    // i4R
                512,    // i4G
                904    // i4B
            },
            // D65 
            {
                976,    // i4R
                512,    // i4G
                735    // i4B
            },
            // DF 
            {
                1090,    // i4R
                512,    // i4G
                889    // i4B
            }
        },
        // Rotation matrix parameter
        {
            2,    // i4RotationAngle
            256,    // i4Cos
            9    // i4Sin
        },
        // Daylight locus parameter
        {
            -133,    // i4SlopeNumerator
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
            -233,    // i4RightBound
            -883,    // i4LeftBound
            -307,    // i4UpperBound
            -407    // i4LowerBound
            },
            // Warm fluorescent
            {
            -233,    // i4RightBound
            -883,    // i4LeftBound
            -407,    // i4UpperBound
            -527    // i4LowerBound
            },
            // Fluorescent
            {
            -52,    // i4RightBound
            -233,    // i4LeftBound
            -301,    // i4UpperBound
            -458    // i4LowerBound
            },
            // CWF
            {
            -52,    // i4RightBound
            -233,    // i4LeftBound
            -458,    // i4UpperBound
            -585    // i4LowerBound
            },
            // Daylight
            {
            167,    // i4RightBound
            -52,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
            },
            // Shade
            {
            477,    // i4RightBound
            167,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            167,    // i4RightBound
            -52,    // i4LeftBound
            -458,    // i4UpperBound
            -585    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            477,    // i4RightBound
            -883,    // i4LeftBound
            0,    // i4UpperBound
            -585    // i4LowerBound
            },
            // Daylight
            {
            182,    // i4RightBound
            -52,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
            },
            // Cloudy daylight
            {
            282,    // i4RightBound
            107,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
            },
            // Shade
            {
            382,    // i4RightBound
            107,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
            },
            // Twilight
            {
            -52,    // i4RightBound
            -212,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
            },
            // Fluorescent
            {
            142,    // i4RightBound
            -283,    // i4LeftBound
            -326,    // i4UpperBound
            -585    // i4LowerBound
            },
            // Warm fluorescent
            {
            -267,    // i4RightBound
            -467,    // i4LeftBound
            -326,    // i4UpperBound
            -585    // i4LowerBound
            },
            // Incandescent
            {
            -267,    // i4RightBound
            -467,    // i4LeftBound
            -296,    // i4UpperBound
            -458    // i4LowerBound
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
            945,    // i4R
            512,    // i4G
            764    // i4B
            },
            // Cloudy daylight
            {
            1118,    // i4R
            512,    // i4G
            638    // i4B
            },
            // Shade
            {
            1194,    // i4R
            512,    // i4G
            595    // i4B
            },
            // Twilight
            {
            731,    // i4R
            512,    // i4G
            1007    // i4B
            },
            // Fluorescent
            {
            884,    // i4R
            512,    // i4G
            1024    // i4B
            },
            // Warm fluorescent
            {
            600,    // i4R
            512,    // i4G
            1550    // i4B
            },
            // Incandescent
            {
            538,    // i4R
            512,    // i4G
            1399    // i4B
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
            7473    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5429    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1886    // i4OffsetThr
            },
            // Daylight WB gain
            {
            864,    // i4R
            512,    // i4G
            839    // i4B
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
                -595,    // i4RotatedXCoordinate[0]
                -459,    // i4RotatedXCoordinate[1]
                -275,    // i4RotatedXCoordinate[2]
                -94,    // i4RotatedXCoordinate[3]
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


