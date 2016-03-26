#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov12830raw.h"
#include "camera_info_ov12830raw.h"
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
        72500,    // i4R_AVG
        16273,    // i4R_STD
        89050,    // i4B_AVG
        17422,    // i4B_STD
        {  // i4P00[9]
            5075000, -2490000, -27500, -722500, 3772500, -482500, -210000, -2407500, 5180000
        },
        {  // i4P10[9]
            1901461, -2392710, 492988, -241310, -157820, 363853, 150533, 190794, -322963
        },
        {  // i4P01[9]
            1293335, -1635923, 340291, -394229, -211377, 575029, -44540, -533825, 596527
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
            90,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            18,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            18,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            22,    // u4CapExpUnit 
            15,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
 	      22,//Lenovo-sw zhangjiano modified 2013-01-24,for A820 camera//24,      // u4LensFno, Fno = 2.8
            354//Lenovo-sw zhangjiano modified 2013-01-24,for A820 camera//350     // u4FocusLength_100x
         },
         // rHistConfig
        {
            4,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {82, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
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
            55,    // u4AETarget
            55,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            4,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -12,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            32,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            160,    // u4PrvMaxFlareThres
            5,    // u4PrvMinFlareThres
            160,    // u4VideoMaxFlareThres
            5,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            60    // u4FlatnessStrength
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
                847,    // i4R
                512,    // i4G
                678    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                82,    // i4X
                -290    // i4Y
            },
            // Horizon
            {
                -423,    // i4X
                -261    // i4Y
            },
            // A
            {
                -302,    // i4X
                -315    // i4Y
            },
            // TL84
            {
                -148,    // i4X
                -343    // i4Y
            },
            // CWF
            {
                -121,    // i4X
                -381    // i4Y
            },
            // DNP
            {
                -21,    // i4X
                -350    // i4Y
            },
            // D65
            {
                82,    // i4X
                -290    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                77,    // i4X
                -291    // i4Y
            },
            // Horizon
            {
                -427,    // i4X
                -254    // i4Y
            },
            // A
            {
                -307,    // i4X
                -310    // i4Y
            },
            // TL84
            {
                -153,    // i4X
                -341    // i4Y
            },
            // CWF
            {
                -127,    // i4X
                -379    // i4Y
            },
            // DNP
            {
                -26,    // i4X
                -350    // i4Y
            },
            // D65
            {
                77,    // i4X
                -291    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                847,    // i4R
                512,    // i4G
                678    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                637,    // i4G
                1610    // i4B
            },
            // A 
            {
                521,    // i4R
                512,    // i4G
                1180    // i4B
            },
            // TL84 
            {
                667,    // i4R
                512,    // i4G
                996    // i4B
            },
            // CWF 
            {
                729,    // i4R
                512,    // i4G
                1010    // i4B
            },
            // DNP 
            {
                799,    // i4R
                512,    // i4G
                846    // i4B
            },
            // D65 
            {
                847,    // i4R
                512,    // i4G
                678    // i4B
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
            1,    // i4RotationAngle
            256,    // i4Cos
            4    // i4Sin
        },
        // Daylight locus parameter
        {
            -130,    // i4SlopeNumerator
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
            -203,    // i4RightBound
            -853,    // i4LeftBound
            -229,    // i4UpperBound
            -335    // i4LowerBound
            },
            // Warm fluorescent
            {
            -203,    // i4RightBound
            -853,    // i4LeftBound
            -335,    // i4UpperBound
            -455    // i4LowerBound
            },
            // Fluorescent
            {
            -76,    // i4RightBound
            -203,    // i4LeftBound
            -220,    // i4UpperBound
            -360    // i4LowerBound
            },
            // CWF
            {
            -76,    // i4RightBound
            -203,    // i4LeftBound
            -360,    // i4UpperBound
            -460    // i4LowerBound
            },
            // Daylight
            {
            102,    // i4RightBound
            -76,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
            },
            // Shade
            {
            462,    // i4RightBound
            102,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            102,    // i4RightBound
            -76,    // i4LeftBound
            -375,    // i4UpperBound
            -475    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            462,    // i4RightBound
            -853,    // i4LeftBound
            0,    // i4UpperBound
            -475    // i4LowerBound
            },
            // Daylight
            {
            127,    // i4RightBound
            -76,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
            },
            // Cloudy daylight
            {
            227,    // i4RightBound
            52,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
            },
            // Shade
            {
            327,    // i4RightBound
            52,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
            },
            // Twilight
            {
            -76,    // i4RightBound
            -236,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
            },
            // Fluorescent
            {
            127,    // i4RightBound
            -253,    // i4LeftBound
            -241,    // i4UpperBound
            -429    // i4LowerBound
            },
            // Warm fluorescent
            {
            -207,    // i4RightBound
            -407,    // i4LeftBound
            -241,    // i4UpperBound
            -429    // i4LowerBound
            },
            // Incandescent
            {
            -207,    // i4RightBound
            -407,    // i4LeftBound
            -211,    // i4UpperBound
            -375    // i4LowerBound
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
            792,    // i4R
            512,    // i4G
            730    // i4B
            },
            // Cloudy daylight
            {
            922,    // i4R
            512,    // i4G
            624    // i4B
            },
            // Shade
            {
            986,    // i4R
            512,    // i4G
            583    // i4B
            },
            // Twilight
            {
            622,    // i4R
            512,    // i4G
            937    // i4B
            },
            // Fluorescent
            {
            746,    // i4R
            512,    // i4G
            872    // i4B
            },
            // Warm fluorescent
            {
            539,    // i4R
            512,    // i4G
            1220    // i4B
            },
            // Incandescent
            {
            509,    // i4R
            512,    // i4G
            1154    // i4B
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
            6177    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            4815    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1341    // i4OffsetThr
            },
            // Daylight WB gain
            {
            738,    // i4R
            512,    // i4G
            782    // i4B
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
            525,    // i4G
            525    // i4B
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
                -504,    // i4RotatedXCoordinate[0]
                -384,    // i4RotatedXCoordinate[1]
                -230,    // i4RotatedXCoordinate[2]
                -103,    // i4RotatedXCoordinate[3]
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


