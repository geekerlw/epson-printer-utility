/*________________________________  epsmp-prn-parse.c  _________________________________*/

/*       1         2         3         4         5         6         7         8        */
/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*******************************************|********************************************/
/*
 *   Copyright (c) 2007  Seiko Epson Corporation                 All rights reserved.
 *
 *   Copyright protection claimed includes all forms and matters of
 *   copyrightable material and information now allowed by statutory or judicial
 *   law or hereinafter granted, including without limitation, material generated
 *   from the software programs which are displayed on the screen such as icons,
 *   screen display looks, etc.
 *
 */
/*******************************************|********************************************/
/*                                                                                      */
/*                          Epson Parse Script File Functions                           */
/*                                                                                      */
/*                                    Function Calls                                    */
/*                               --------------------------                             */
/*    EPS_ERR_CODE epsmpGetPageAttribute (ScriptFileName, ImageBand, PageAttribute);    */
/*    EPS_INT32    epsmpGetScriptLineNum (void);                                        */
/*                                                                                      */
/*******************************************|********************************************/

/*------------------------------------  Includes   -------------------------------------*/
/*******************************************|********************************************/
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "epsmp-prn-parse.h"
#include "epsmp-image.h"

/*------------------------------------- Data Types -------------------------------------*/
/*******************************************|********************************************/
typedef EPS_INT32   KEY;
typedef EPS_INT32   VALUE;

/*------------------------------------  Definition   -----------------------------------*/
/*******************************************|********************************************/
#define MAX_STRING 256

    /*** max number of parameters in a comma separated list                             */
    /*** -------------------------------------------------------------------------------*/
#define MAX_PARAM       10

    /*** ascii keys that need to be converted using ValuesTbl                           */
    /*** -------------------------------------------------------------------------------*/
typedef enum _tagEPS_KEY {
    KEY_MEDIA_SIZE,
    KEY_MEDIA_TYPE,
    KEY_BORDER,
    KEY_QUALITY,
    KEY_COLOR,
    KEY_TOP,
    KEY_LEFT,
    KEY_BOTTOM,
    KEY_RIGHT,
    KEY_START,
    KEY_END,
    KEY_UNKNOWN,
} EPS_KEY;

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/
    /*** mapping from ascii keys to key/value/string constants                          */
    /*** -------------------------------------------------------------------------------*/
static const struct _tag_EPS_VALUE {
    const KEY       key;        /* key */
    const VALUE     value;      /* value */
    const EPS_INT8* str;        /* string */
} ValuesTbl[] = {
    
    /* Color Mono */
    { KEY_COLOR,        EPS_CM_COLOR,           "EPS_CM_COLOR"          },
    { KEY_COLOR,        EPS_CM_MONOCHROME,      "EPS_CM_MONOCHROME"     },

    /* Quality */
    { KEY_QUALITY,      EPS_MQID_DRAFT,         "EPS_PQ_DRAFT"        },
    { KEY_QUALITY,      EPS_MQID_NORMAL,        "EPS_PQ_NORMAL"       },
    { KEY_QUALITY,      EPS_MQID_HIGH,          "EPS_PQ_HIGH"         },

    /* Paper Size */
    { KEY_MEDIA_SIZE,   EPS_MSID_A4,            "EPS_MSID_A4"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_LETTER,        "EPS_MSID_LETTER"       },
    { KEY_MEDIA_SIZE,   EPS_MSID_LEGAL,         "EPS_MSID_LEGAL"        },
    { KEY_MEDIA_SIZE,   EPS_MSID_A5,            "EPS_MSID_A5"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_A6,            "EPS_MSID_A6"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_B5,            "EPS_MSID_B5"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_EXECUTIVE,     "EPS_MSID_EXECUTIVE"    },
    { KEY_MEDIA_SIZE,   EPS_MSID_HALFLETTER,    "EPS_MSID_HALFLETTER"   },
    { KEY_MEDIA_SIZE,   EPS_MSID_PANORAMIC,     "EPS_MSID_PANORAMIC"    },
    { KEY_MEDIA_SIZE,   EPS_MSID_TRIM_4X6,      "EPS_MSID_TRIM_4X6"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_4X6,           "EPS_MSID_4X6"          },
    { KEY_MEDIA_SIZE,   EPS_MSID_5X8,           "EPS_MSID_5X8"          },
    { KEY_MEDIA_SIZE,   EPS_MSID_8X10,          "EPS_MSID_8X10"         },
    { KEY_MEDIA_SIZE,   EPS_MSID_10X15,         "EPS_MSID_10X15"        },
    { KEY_MEDIA_SIZE,   EPS_MSID_200X300,       "EPS_MSID_200X300"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_L,             "EPS_MSID_L"            },
    { KEY_MEDIA_SIZE,   EPS_MSID_POSTCARD,      "EPS_MSID_POSTCARD"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_DBLPOSTCARD,   "EPS_MSID_DBLPOSTCARD"  },
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_10_L,      "EPS_MSID_ENV_10_L"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_C6_L,      "EPS_MSID_ENV_C6_L"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_DL_L,      "EPS_MSID_ENV_DL_L"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_NEWEVN_L,      "EPS_MSID_NEWEVN_L"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_CHOKEI_3,      "EPS_MSID_CHOKEI_3"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_CHOKEI_4,      "EPS_MSID_CHOKEI_4"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_YOKEI_1,       "EPS_MSID_YOKEI_1"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_YOKEI_2,       "EPS_MSID_YOKEI_2"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_YOKEI_3,       "EPS_MSID_YOKEI_3"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_YOKEI_4,       "EPS_MSID_YOKEI_4"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_2L,            "EPS_MSID_2L"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_10_P,      "EPS_MSID_ENV_10_P"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_C6_P,      "EPS_MSID_ENV_C6_P"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_DL_P,      "EPS_MSID_ENV_DL_P"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_NEWENV_P,      "EPS_MSID_NEWENV_P"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_MEISHI,        "EPS_MSID_MEISHI"       },
    { KEY_MEDIA_SIZE,   EPS_MSID_BUZCARD_89X50, "EPS_MSID_BUZCARD_89X50"},
    { KEY_MEDIA_SIZE,   EPS_MSID_CARD_54X86,    "EPS_MSID_CARD_54X86"   },
    { KEY_MEDIA_SIZE,   EPS_MSID_BUZCARD_55X91, "EPS_MSID_BUZCARD_55X91"},
    { KEY_MEDIA_SIZE,   EPS_MSID_ALBUM_L,       "EPS_MSID_ALBUM_L"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_ALBUM_A5,      "EPS_MSID_ALBUM_A5"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_PALBUM_L_L,    "EPS_MSID_PALBUM_L_L"   },
    { KEY_MEDIA_SIZE,   EPS_MSID_PALBUM_2L,     "EPS_MSID_PALBUM_2L"    },
    { KEY_MEDIA_SIZE,   EPS_MSID_PALBUM_A5_L,   "EPS_MSID_PALBUM_A5_L"  },
    { KEY_MEDIA_SIZE,   EPS_MSID_PALBUM_A4,     "EPS_MSID_PALBUM_A4"    },
    { KEY_MEDIA_SIZE,   EPS_MSID_HIVISION,      "EPS_MSID_HIVISION"     },
    { KEY_MEDIA_SIZE,   EPS_MSID_KAKU_2,		"EPS_MSID_KAKU_2"		},
    { KEY_MEDIA_SIZE,   EPS_MSID_ENV_C4_P,		"EPS_MSID_ENV_C4_P"		},
    { KEY_MEDIA_SIZE,   EPS_MSID_B6,            "EPS_MSID_B6"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_KAKU_20,       "EPS_MSID_KAKU_20"      },
    { KEY_MEDIA_SIZE,   EPS_MSID_A3NOBI,        "EPS_MSID_A3NOBI"       },
    { KEY_MEDIA_SIZE,   EPS_MSID_A3,            "EPS_MSID_A3"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_B4,            "EPS_MSID_B4"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_USB,           "EPS_MSID_USB"          },
    { KEY_MEDIA_SIZE,   EPS_MSID_11X14,         "EPS_MSID_11X14"        },
    { KEY_MEDIA_SIZE,   EPS_MSID_B3,            "EPS_MSID_B3"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_A2,            "EPS_MSID_A2"           },
    { KEY_MEDIA_SIZE,   EPS_MSID_USC,           "EPS_MSID_USC"          },
    { KEY_MEDIA_SIZE,   EPS_MSID_10X12,         "EPS_MSID_10X12"        },
    { KEY_MEDIA_SIZE,   EPS_MSID_12X12,         "EPS_MSID_12X12"        },
    { KEY_MEDIA_SIZE,   EPS_MSID_USER,          "EPS_MSID_USER"         },
    
    /* Layout setting */
    { KEY_BORDER,       EPS_MLID_BORDERS,       "EPS_BM_FIXED"      	},
    { KEY_BORDER,       EPS_MLID_BORDERLESS,    "EPS_BM_BORDERLESS"		},
    { KEY_BORDER,       EPS_MLID_CDLABEL,       "EPS_BM_CDLABEL"		},
    { KEY_BORDER,       EPS_MLID_CUSTOM,        "EPS_BM_CUSTOM"			},
    
    /* Media Type */
    { KEY_MEDIA_TYPE,   EPS_MTID_PLAIN,         "EPS_MTID_PLAIN"        },
    { KEY_MEDIA_TYPE,   EPS_MTID_360INKJET,     "EPS_MTID_360INKJET"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_IRON,          "EPS_MTID_IRON"         },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOINKJET,   "EPS_MTID_PHOTOINKJET"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOADSHEET,  "EPS_MTID_PHOTOADSHEET" },
    { KEY_MEDIA_TYPE,   EPS_MTID_MATTE,         "EPS_MTID_MATTE"        },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTO,         "EPS_MTID_PHOTO"        },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOFILM,     "EPS_MTID_PHOTOFILM"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_MINIPHOTO,     "EPS_MTID_MINIPHOTO"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_OHP,           "EPS_MTID_OHP"          },
    { KEY_MEDIA_TYPE,   EPS_MTID_BACKLIGHT,     "EPS_MTID_BACKLIGHT"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_PGPHOTO,       "EPS_MTID_PGPHOTO"      },
    { KEY_MEDIA_TYPE,   EPS_MTID_PSPHOTO,       "EPS_MTID_PSPHOTO"      },
    { KEY_MEDIA_TYPE,   EPS_MTID_PLPHOTO,       "EPS_MTID_PLPHOTO"      },
    { KEY_MEDIA_TYPE,   EPS_MTID_MCGLOSSY,      "EPS_MTID_MCGLOSSY"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_ARCHMATTE,     "EPS_MTID_ARCHMATTE"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_WATERCOLOR,    "EPS_MTID_WATERCOLOR"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_PROGLOSS,      "EPS_MTID_PROGLOSS"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_MATTEBOARD,    "EPS_MTID_MATTEBOARD"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOGLOSS,    "EPS_MTID_PHOTOGLOSS"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_SEMIPROOF,     "EPS_MTID_SEMIPROOF"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_SUPERFINE2,    "EPS_MTID_SUPERFINE2"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_DSMATTE,       "EPS_MTID_DSMATTE"      },
    { KEY_MEDIA_TYPE,   EPS_MTID_CLPHOTO,       "EPS_MTID_CLPHOTO"      },
    { KEY_MEDIA_TYPE,   EPS_MTID_ECOPHOTO,      "EPS_MTID_ECOPHOTO"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_VELVETFINEART, "EPS_MTID_VELVETFINEART"},
    { KEY_MEDIA_TYPE,   EPS_MTID_PROOFSEMI,     "EPS_MTID_PROOFSEMI"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_HAGAKIRECL,    "EPS_MTID_HAGAKIRECL"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_HAGAKIINKJET,  "EPS_MTID_HAGAKIINKJET" },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOINKJET2,  "EPS_MTID_PHOTOINKJET2" },
    { KEY_MEDIA_TYPE,   EPS_MTID_DURABRITE,     "EPS_MTID_DURABRITE"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_MATTEMEISHI,   "EPS_MTID_MATTEMEISHI"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_HAGAKIATENA,   "EPS_MTID_HAGAKIATENA"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOALBUM,    "EPS_MTID_PHOTOALBUM"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOSTAND,    "EPS_MTID_PHOTOSTAND"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_RCB,           "EPS_MTID_RCB"          },
    { KEY_MEDIA_TYPE,   EPS_MTID_PGPHOTOEG,     "EPS_MTID_PGPHOTOEG"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_ENVELOPE,      "EPS_MTID_ENVELOPE"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_PLATINA,       "EPS_MTID_PLATINA"      },
    { KEY_MEDIA_TYPE,   EPS_MTID_ULTRASMOOTH,   "EPS_MTID_ULTRASMOOTH"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_SFHAGAKI,      "EPS_MTID_SFHAGAKI"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_PHOTOSTD,      "EPS_MTID_PHOTOSTD"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_GLOSSYHAGAKI,  "EPS_MTID_GLOSSYHAGAKI" },
    { KEY_MEDIA_TYPE,   EPS_MTID_GLOSSYPHOTO,   "EPS_MTID_GLOSSYPHOTO"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_GLOSSYCAST,    "EPS_MTID_GLOSSYCAST"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_BUSINESSCOAT,  "EPS_MTID_BUSINESSCOAT" },
    { KEY_MEDIA_TYPE,   EPS_MTID_MEDICINEBAG,   "EPS_MTID_MEDICINEBAG"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_THICKPAPER,    "EPS_MTID_THICKPAPER"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_BROCHURE,      "EPS_MTID_BROCHURE"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_MATTE_DS,      "EPS_MTID_MATTE_DS"     },
    { KEY_MEDIA_TYPE,   EPS_MTID_BSMATTE_DS,    "EPS_MTID_BSMATTE_DS"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_3D,            "EPS_MTID_3D"           },
    { KEY_MEDIA_TYPE,   EPS_MTID_LCPP,          "EPS_MTID_LCPP"         },
    { KEY_MEDIA_TYPE,   EPS_MTID_PREPRINTED,    "EPS_MTID_PREPRINTED"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_LETTERHEAD,    "EPS_MTID_LETTERHEAD"   },
    { KEY_MEDIA_TYPE,   EPS_MTID_RECYCLED,      "EPS_MTID_RECYCLED"     },
	{ KEY_MEDIA_TYPE,   EPS_MTID_CDDVD,         "EPS_MTID_CDDVD"        },
    { KEY_MEDIA_TYPE,   EPS_MTID_CDDVDHIGH,     "EPS_MTID_CDDVDHIGH"    },
    { KEY_MEDIA_TYPE,   EPS_MTID_CDDVDGLOSSY,   "EPS_MTID_CDDVDGLOSSY"  },
    { KEY_MEDIA_TYPE,   EPS_MTID_CLEANING,      "EPS_MTID_CLEANING"     },
 
	{ KEY_UNKNOWN,      KEY_UNKNOWN,            ""                      }
};

    /*** mapping from keys to ascii keys                                                */
    /*** -------------------------------------------------------------------------------*/
static const struct _tag_EPS_KEY_VALUE {
    const EPS_INT32 key;
    const EPS_INT8* value;
} KeysTbl[] = {

    {KEY_MEDIA_SIZE,    "SIZE"	        },
    {KEY_MEDIA_TYPE,    "MEDIA_TYPE"    },
    {KEY_BORDER,        "BORDER"        },
    {KEY_QUALITY,       "QUALITY"       },
    {KEY_COLOR,         "COLOR"         },
    {KEY_TOP,           "TOP"           },
    {KEY_LEFT,          "LEFT"          },
    {KEY_BOTTOM,        "BOTTOM"        },
    {KEY_RIGHT,         "RIGHT"         },
    {KEY_START,         "START"         },
    {KEY_END,           "END"           },
    {KEY_UNKNOWN,       ""              }

};

/*------------------------------  Local Global Variables  ------------------------------*/
/*******************************************|********************************************/
static EPS_INT32 gLineNum;

/*---------------------------  Data Structure Declarations   ---------------------------*/
/*******************************************|********************************************/

/*--------------------------------  Local Functions   ----------------------------------*/
/*******************************************|********************************************/
static void         InitPageAttributeParam  (EPS_JOB_ATTRIB*                            );
static EPS_ERR_CODE GetNextKey              (FILE*, KEY*, EPS_INT8*, EPS_INT32*, VALUE* );
static void         RemoveSpace             (EPS_INT8*, const EPS_INT8*                 );

/*------------------------------------  Debug Dump   -----------------------------------*/
/*******************************************|********************************************/
#define PRINT_PAIR(string,value) printf("%s: %d\n",(string),(value));

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpGetPageAttribute()                                             */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* testscript       const EPS_INT8*     I: Script Fine Name                             */
/* pageAttr         EPS_JOB_ATTRIB*     O: Page Attribution                             */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_PRS_ERR_NONE                    - Success                                 */
/*      EPSMP_PRS_ERR_INVALID_ARG             - Invalid argument                        */
/*      EPSMP_PRS_ERR_TESTSCRIPT_IO           - Script file open error                  */
/*      EPSMP_PRS_ERR_INVALID_NUM_PARAM       - invalid number of parameter.            */
/*      EPSMP_PRS_ERR_INVALID_PARAM           - invalid parameter.                      */
/*      EPSMP_PRS_ERR_UNKNOWN_KEY             - unknown key.                            */
/*      EPSMP_PRS_ERR_END_OF_TEST             - test file end is reached.               */
/*                                              (no more lines to read)                 */
/*                                                                                      */
/* Description:                                                                         */
/*      Parse the script file and get the page attribution.                             */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpGetPageAttribute (

        const EPS_INT8*    testscript,
        EPS_JOB_ATTRIB*    pageAttr

){
/*** Declare Variable Local to Routine                                                  */
    KEY         key;                        /* the key we are processing                */
    EPS_INT8    charParam[MAX_STRING];      /* the raw parameter as a string            */
    EPS_INT32   numParam;                   /* number of values in params               */
    VALUE       params[MAX_PARAM];          /* parsed parameters                        */
    EPS_INT8*   endptr;
    
    EPS_ERR_CODE ret           = EPSMP_PRS_ERR_NONE;
    FILE*        fp_testscript = NULL;

/*** Validate input parameters                                                          */
    if (pageAttr == NULL || testscript == NULL) {
        ret = EPSMP_PRS_ERR_INVALID_ARG;
        goto prn_parse_init_ERROR;
    }

    fp_testscript = fopen(testscript,"r");
    
    if (fp_testscript == NULL) {
        ret = EPSMP_PRS_ERR_TESTSCRIPT_IO;
        goto prn_parse_init_ERROR;
    }

/*** Initialize Global/Local Variables                                                  */
    gLineNum = 0;
    key       = 0;
    numParam  = 0;
    memset(charParam, 0, MAX_STRING);
    memset(params,    0, MAX_PARAM );
    
/*** Init param                                                                         */
    InitPageAttributeParam(pageAttr);

/*** Set param                                                                          */
    while (1) {
        ret = GetNextKey(fp_testscript, &key, charParam, &numParam, params);

        if (ret != EPSMP_PRS_ERR_NONE) break;

        switch(key) {
            case KEY_MEDIA_SIZE:
                pageAttr->mediaSizeIdx  = params[0];
                break;
            case KEY_MEDIA_TYPE:
                pageAttr->mediaTypeIdx  = (EPS_INT8)params[0];
                break;
            case KEY_BORDER:
                pageAttr->printLayout   = (EPS_UINT8)params[0];
                break;
            case KEY_QUALITY:
                pageAttr->printQuality  = (EPS_INT8)params[0];
                break;
            case KEY_COLOR:
                pageAttr->colorMode     = (EPS_INT8)params[0];
                break;
            case KEY_TOP:
				pageAttr->topMargin     = (EPS_INT16)strtol(charParam, &endptr, 0);
                break;
            case KEY_LEFT:
                pageAttr->leftMargin    = (EPS_INT16)strtol(charParam, &endptr, 0);
                break;
            case KEY_BOTTOM:
                pageAttr->bottomMargin  = (EPS_INT16)strtol(charParam, &endptr, 0);
                break;
            case KEY_RIGHT:
                pageAttr->rightMargin   = (EPS_INT16)strtol(charParam, &endptr, 0);
                break;
            case KEY_START:
                /* Initialize Structures to Default */
                InitPageAttributeParam(pageAttr);
                break;
            case KEY_END:
                /* Break out of case statement and loop */
                goto LOOP_BREAK;
        }

        if (ret != EPSMP_PRS_ERR_NONE) break;
    }

LOOP_BREAK:
    if (ret != EPSMP_PRS_ERR_NONE) goto prn_parse_init_ERROR;

/*** Check and see if CD/DVD label printing is worked;                                  */
/*** if the user set the border mode to either EPS_MLID_CDLABEL,                        */
/*** then set the dimameter to default value.                                           */
	if( pageAttr->printLayout == EPS_MLID_CDLABEL ){
		pageAttr->cdDimIn  = 43;
		pageAttr->cdDimOut = 116;
    }

    ret = EPSMP_PRS_ERR_NONE;

prn_parse_init_ERROR:
    if (fp_testscript != NULL)
        fclose(fp_testscript);

    return (EPS_ERR_CODE)ret;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpGetPageAttributeFromImage()                                    */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* imageBand        EPSMP_IMAGE_BAND*   I: Image Band Information                       */
/* pageAttr         EPS_JOB_ATTRIB*     O: Page Attribution                             */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_PRS_ERR_NONE                    - Success                                 */
/*                                                                                      */
/* Description:                                                                         */
/*      Get the page attribution from image attribut.                                   */
/*                                                                                      */
/*******************************************|********************************************/
EPS_ERR_CODE    epsmpGetPageAttributeFromImage (

        EPSMP_IMAGE_BAND*   imageBand,
        EPS_JOB_ATTRIB*     pageAttr

){
    EPS_ERR_CODE ret           = EPSMP_PRS_ERR_NONE;

/*** Set Image Attribute                                                                */
    switch( imageBand->imageType ){
	case EPSMP_BMP:
	case EPSMP_JPG:
		/*** RGB print ***/
        pageAttr->colorPlane = EPS_CP_FULLCOLOR;

        if (imageBand->bpp == 1) {
            pageAttr->colorPlane  = EPS_CP_256COLOR;
            pageAttr->paletteSize = imageBand->paletteSize;
            pageAttr->paletteData = imageBand->paletteData;

        /*** Change the palette data from RGB to gray if the user set the monochrome    */
        /*** printing on 8-bit image.                                                   */
            if (pageAttr->colorMode == EPS_CM_MONOCHROME) {
                
                ret = epsmpRGBtoGrayPalette(imageBand);
				if (ret != EPSMP_IMAGE_ERR_NONE){
					goto prn_parse_init_ERROR;
				}
                
                pageAttr->paletteData = imageBand->paletteData;
            }
        }
		break;

	case EPSMP_JPGRAW:
		/*** Jpeg file print ***/
		pageAttr->colorPlane = EPS_CP_JPEG;
		break;

	default:
		ret = EPS_ERR_INV_COLOR_PLANE;        
		break;
    }

prn_parse_init_ERROR:
	return (EPS_ERR_CODE)ret;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   epsmpGetScriptLineNum()                                             */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:                   Description:                                */
/* -                -                       -                                           */
/*                                                                                      */
/* Return value:                                                                        */
/*      Line Number of Script File                                                      */
/*                                                                                      */
/* Description:                                                                         */
/*      Return the line number of the script file.                                      */
/*                                                                                      */
/*******************************************|********************************************/
EPS_INT32   epsmpGetScriptLineNum (

        void

){

    return gLineNum;

}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*--------------------             Internal Processing             ---------------------*/
/*%%%%%%%%%%%%%%%%%%%%                                             %%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   InitPageAttributeParam()                                            */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:                   Description:                                */
/* pageAttr         EPS_JOB_ATTRIB*         Page Attribution                            */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Init the page attirbution structures to default values.                         */
/*                                                                                      */
/*******************************************|********************************************/
static void InitPageAttributeParam (

        EPS_JOB_ATTRIB*    pageAttr

){

    pageAttr->mediaSizeIdx      = EPS_MSID_L;
    pageAttr->mediaTypeIdx      = EPS_MTID_PGPHOTO;
    pageAttr->printLayout       = EPS_MLID_BORDERS;
    pageAttr->printQuality      = EPS_MQID_HIGH;
    pageAttr->colorMode         = EPS_CM_COLOR;
    pageAttr->inputResolution   = EPS_IR_360X360;
	pageAttr->printDirection    = EPS_PD_BIDIREC;
    pageAttr->colorPlane        = EPS_CP_FULLCOLOR;  
    pageAttr->paletteSize       = 0;  
    pageAttr->paletteData       = NULL;  
    pageAttr->brightness        = 0;
    pageAttr->contrast          = 0;
    pageAttr->saturation        = 0;
    pageAttr->topMargin         = 280;
    pageAttr->leftMargin        = 280;
    pageAttr->bottomMargin      = 280;
    pageAttr->rightMargin       = 280;
}


/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   GetNextKey()                                                        */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:               Description:                                    */
/* fp_testscript    FILE*               I: test script                                  */
/* key              KEY*                O: the next key                                 */
/* p0               EPS_INT8*           O: the raw parameter value as a string          */
/*                                         (surrounding whitespace removed)             */
/* numParam     EPS_INT32*              O: number of parameters in params               */
/* params           VALUE*              O: the raw paramater value parsed as a comma    */
/*                                         separated list.                              */
/*                                         (ascii parameters automatically converted to */
/*                                          their decimal keys)                         */
/*                                                                                      */
/* Return value:                                                                        */
/*      EPSMP_PRS_ERR_NONE                    - Success                                 */
/*      EPSMP_PRS_ERR_INVALID_NUM_PARAM       - invalid number of parameter.            */
/*      EPSMP_PRS_ERR_INVALID_PARAM           - invalid parameter.                      */
/*      EPSMP_PRS_ERR_UNKNOWN_KEY             - unknown key.                            */
/*      EPSMP_PRS_ERR_END_OF_TEST             - test file end is reached.               */
/*                                              (no more lines to read)                 */
/*                                                                                      */
/* Description:                                                                         */
/*      Gets the next key in the test file.                                             */
/*                                                                                      */
/*******************************************|********************************************/
static EPS_ERR_CODE     GetNextKey (

        FILE*       fp_testscript,
        KEY*        key,
        EPS_INT8*   p0,
        EPS_INT32*  numParam,
        VALUE*      params

){

/*** Declare Variable Local to Routine                                                  */
    EPS_INT8    line[MAX_STRING];
    EPS_INT8    buffer[MAX_STRING];
    EPS_INT32   i;

    EPS_INT8*   str_key;
    EPS_INT8*   str_param;

/*** Initialize Local Variables                                                         */
    *numParam = 0;
    memset(buffer, 0, MAX_STRING);

/*** read next line                                                                     */
    while (1) {
        if (feof(fp_testscript)) return EPSMP_PRS_ERR_END_OF_TEST;

        if (fgets(line,MAX_STRING-1,fp_testscript) == NULL)
            return EPSMP_PRS_ERR_UNKNOWN_KEY;
        
        RemoveSpace(buffer,line);

        gLineNum++;

        /* skip comments and empty lines */
        if (buffer[0] != '#' && strlen(buffer) != 0) {
            break;
        }
    }

/*** Get the key all characters before ':'                                              */
    str_key   = strtok(buffer,":"); 
    str_param = strtok(NULL,":");


    if (str_key == NULL) return EPSMP_PRS_ERR_UNKNOWN_KEY;

    if (str_param != NULL) {
        strcpy(p0,str_param);
    } else {
        strcpy(p0,"");
    }

/*** find the key in table                                                              */
    for (i=0; KeysTbl[i].key != KEY_UNKNOWN; i++) {
        if (strcmp(KeysTbl[i].value,str_key) == 0) {
            break;
        }
    }

    if (KeysTbl[i].key == KEY_UNKNOWN) return EPSMP_PRS_ERR_UNKNOWN_KEY;

    *key = KeysTbl[i].key;

    switch (*key) {

            /* ascii keys that need to be converted using ValuesTbl */
            case KEY_MEDIA_TYPE:
            case KEY_MEDIA_SIZE:
            case KEY_QUALITY:
            case KEY_BORDER:
            case KEY_COLOR:

                for (i=0; ValuesTbl[i].key != KEY_UNKNOWN; i++) {
                    if (strcmp(ValuesTbl[i].str,str_param) == 0 && *key == ValuesTbl[i].key) {
                        params[*numParam] = ValuesTbl[i].value;
                        (*numParam)++;
                        break;
                    }
                }

                if (*numParam == 0) return EPSMP_PRS_ERR_INVALID_PARAM;
                break;
            /* do nothing */
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_TOP:
            case KEY_BOTTOM:
            case KEY_START:
            case KEY_END:
                break;
        }

/*** Return to Caller                                                                   */
    return EPSMP_PRS_ERR_NONE;
}

/*******************************************|********************************************/
/*                                                                                      */
/* Function name:   RemoveSpace()                                                       */
/*                                                                                      */
/* Arguments                                                                            */
/* ---------                                                                            */
/* Name:            Type:                   Description:                                */
/* dest             EPS_INT8*               O: Output Character String                  */
/* src              const EPS_INT8*         I: Input Character String                   */
/*                                                                                      */
/* Return value:                                                                        */
/*      None                                                                            */
/*                                                                                      */
/* Description:                                                                         */
/*      Remove all space characters from src and return result in dest.                 */
/*                                                                                      */
/*******************************************|********************************************/
static void     RemoveSpace (

        EPS_INT8*           dest,
        const EPS_INT8*     src

){

    EPS_INT32 i;
    EPS_INT32 dest_index = 0;

    for (i=0; src[i] != '\0'; i++) {
        if (!isspace(src[i])) {
            dest[dest_index] = src[i];
            dest_index++;
        }
    }
    dest[dest_index] = '\0';
}   


/*________________________________  epsmp-prn-parse.c  _________________________________*/

/*34567890123456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*       1         2         3         4         5         6         7         8        */
/*******************************************|********************************************/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/***** End of File *** End of File *** End of File *** End of File *** End of File ******/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%|%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
