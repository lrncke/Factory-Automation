/*-------------------------------------------------------------------*
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Control_III                                        *
 *   module name: asi5.h                                             *
 *        author: Christian Sommerfeld                               *
 *          date: 2019-11-18                                         *
 *                                                                   *
 *  version: 1.0 first version                                       *
 *                                                                   *
 *-------------------------------------------------------------------*/
#ifndef ASI5_H_
#define ASI5_H_

/*! ORDS request / response data
 **/
typedef struct odc_data_t_tag
{
    /*! 1 byte command and 239 bytes command data
     **/
     unsigned char *data;
    /*! Length of Data in bytes: 1...239
     **/
     unsigned char dataLength;
    /*! port number where the command is assigned to
     **/
     unsigned char portNumber;
} odc_data_t;

/*! The AS-i 5 identifier is a unique marking for an AS-i 5 slave and AS-i 5
 * master.
 **/
typedef struct asiid_t_tag
{
    /*! The vendor id is a unique number which is defined by the AS-i Verein.
     **/
	unsigned short vendorId;
    /*! The product id is a unique number which is defined by the vendor.
     **/
	unsigned char productId[5];
} asiid_t;

/*! This structure contains the status flag and the status code of one AS-i 5 Slave
 **/
typedef struct asi_5_lpf_t_tag
{
    /*! 0: ASI5Saddr is not in ASI5LPF
     * !0: ASI5Saddr is in ASI5LPF
     **/
     unsigned char in_lpf;
    /*! AS-i 5 slave status code
     * 0: None / Gray / No status information available
     * 1: OK / Green / No action necessary
     * 2: Warning / Yellow / Action recommended
     * 3: Critical / Orange / Action strongly recommended
     * 4: Defect / Red / Action necessary
     **/
     unsigned char sts_code;
} asi_5_lpf_t;

/*! The array contains the actual copies of all AS-i 5 slaves, determined by
 * reading of this data from the AS-i 5 slaves. The items of this image can be
 * accessed by using the logical address of the corresponding logical address.
 **/
typedef struct asi_5_slv_config_t_tag
{
    /*! An AS-i 5 slave profi le ID is number to identify a standardized AS-i 5
     * slave type.
     **/
     unsigned int profile_id;
    /*! AS-i 5 slave vendor ID, if the AS-i 5 slave profile ID is a vendor specific
     * slave profile ID
     **/
     unsigned short vendor_id;
    /*! AS-i 5 slave energy saving group.
     * range: [0...127]
     **/
     unsigned char EnSvGrp;
    /*! AS-i 5 slave prioritisation configuration
     **/
     unsigned char prio;
    /*! Process data watchdog of the AS-i 5 slave.
     **/
     unsigned char PDWdog;
} asi_5_slv_config_t;

/*! This structure contains the logical Address and the status code of one AS-i 5
 * Slave
 **/
typedef struct asi_5_lpf_list_t_tag
{
    /*! logical Address of the Slave in the LPF
     **/
     unsigned short logAddr;
    /*! AS-i 5 slave status code
     * 0: None / Gray / No status information available
     * 1: OK / Green / No action necessary
     * 2: Warning / Yellow / Action recommended
     * 3: Critical / Orange / Action strongly recommended
     * 4: Defect / Red / Action necessary
     **/
     unsigned char sts_code;
} asi_5_lpf_list_t;

/*!
 **/
typedef struct cm_systemResources_t_tag
{
    /*! Number of used transport channel in subcycle [0..3]
     **/
     unsigned char TC[4];
    /*! Number of detected AS-i 5 Slaves (NUM_DET_SLV)
     **/
     unsigned char numDetSlv;
    /*! Number of active AS-i 5 Slaves (NUM_ACT_SLV)
     **/
     unsigned char numActSlv;
    /*! Number of used bytes in the process data image of all detected AS-i 5
     * Slaves (USED_DET_PDI)
     **/
     unsigned short usedDetPDI;
    /*! Number of used bytes in the process data image of all activated AS-i 5
     * Slaves (USED_ACT_PDI)
     **/
     unsigned short usedActPDI;
    /*! Size of process data image, based on detected AS-i 5 Slaves
     * (SIZEOF_DET_PDI)
     **/
     unsigned short sizeOfDetPDI;
    /*! Size of process data image, based on activated AS-i 5 Slaves
     * (SIZEOF_ACT_PDI)
     **/
     unsigned short sizeOfActPDI;
} cm_systemResources_t;

/*! The AS-i 5 slave info (ASI5SLVInfo) is a data structure which contains all
 * available information of a certain AS-i 5 Slave. The structure or parts of the
 * structure can be read out by using the corresponding host interface functions.
 **/
typedef struct asi_5_slvinfo_t_tag
{
    /*! The ASIID of the AS-i 5 slave.
     **/
     asiid_t ASIID;
    /*! logical slave address
     **/
     unsigned short logAddr;
    /*! The active AS-i 5 slave profi le ID. This could be different from the AS-i
     * 5 slave profi le ID which is projected in the AS-i 5 master
     **/
     unsigned int profileID;
    /*! The actual process data type configuration of the AS-i 5 slave. This could
     * be different from the process data type configuration which is projected in
     * the AS-i 5 master.
     **/
     unsigned char pdtCfg[32];
    /*! The actual transport channel type configuration of the AS-i 5 slave. This
     * could be different from the transport channel type configuration which is
     * projected in the AS-i 5 master.
     **/
     unsigned short tcCfg[8];
    /*! The prioritisation configuration of the AS-i 5 slave.
     **/
     unsigned char prio;
    /*! AS-i 5 slave energy saving group.
     * range: [0...127]
     **/
     unsigned char EnSvGrp;
    /*! AS-i 5 slave energy saving state of the AS-i 5 slave energy saving group.
     * range: [0...1]
     **/
     unsigned char EnSvState;
    /*! Process data watchdog of the AS-i 5 slave.
     **/
     unsigned char PDWdog;
    /*! The actual AS-i 5 slave status code.
     **/
     unsigned char slvStatusCode;
    /*! This Flag is set if the AS-i 5 slave is detected by the AS-i 5 master.
     **/
     unsigned char in_asi5lds;
    /*! This Flag is set if the AS-i 5 slave has been activated by the AS-i 5
     * master and capable of communicating properly with the AS-i 5 master.
     **/
     unsigned char in_asi5las;
    /*! This Flag is set if the AS-i 5 slave is projected to the AS-i 5 master.
     **/
     unsigned char in_asi5lps;
    /*! This Flag is set if the AS-i 5 slave has an overlapped process image with
     * another AS-i Slave.
     **/
     unsigned char in_asi5lms;
    /*! This Flag is set if the logical address of AS-i 5 slave is assigned to two
     * or more detected AS-i 5 slaves.
     **/
     unsigned char in_asi5lda;
    /*! This Flag is set if the AS-i 5 slave parameter image is inconsistent with
     * the AS-i 5 master stored parameter image for the AS-i 5 slave.
     **/
     unsigned char in_asi5lis;
    /*! This Flag is set if the logical address of AS-i 5 slave is assigned to the
     * list of AS-i 5 slaves with non-unique Device Designator.
     **/
     unsigned char in_asi5ldd;
    /*! The device designator of the AS-i 5 Slave.
     **/
     unsigned char deviceDesignator[32];
    /*! The communication ID of the AS-i 5 Slave.
     **/
     unsigned char comID;
} asi_5_slvinfo_t;

/*! Source of the diagnostic condition
 **/
typedef enum global_source_t_tag
{
    /*! Diagnostic condition from master
     **/
	master_e,
    /*! Diagnostic condition from slave
     **/
	slave_e
} global_source_t;

/*! Type of the diagnostic condition
 **/
typedef enum diag_type_t_tag
{
    /*!
     **/
	reserved_e = 0,
    /*!
     **/
	rising_e = 1,
    /*!
     **/
	leaving_e = 2,
    /*!
     **/
	singleShot_e = 3
} diag_type_t;

/*! Relevance of the diagnostic condition
 **/
typedef enum diag_relevance_t_tag
{
    /*! Relevance not available
     **/
	unavailable_e = 0,
    /*! Relevance not available
     **/
	information_e = 1,
    /*! Warning
     **/
	warning_e = 2,
    /*! Critical
     **/
	critical_e = 3,
    /*! Critical
     **/
	defect_e = 4
} diag_relevance_t;

/*! Structure of an item of diagnostic information
 **/
typedef struct diag_info_t_tag
{
    /*! system clock value of the occurrence of the described diagnostic condition
     **/
	unsigned long long systemClk;
    /*! Time in milliseconds since 01.01.1970 00:00:00.000.
     * 0 is a reserved value, if no absolute time has been set yet.
     **/
	unsigned long long absoluteTime;
    /*! Source of the diagnostic condition.
     * \attention This should be renamed to diagnosticHeader!
     **/
     global_source_t globalSource;
    /*! specific source of the diagnostic condition
     * if globalSource=slave_e: logical address of the reporting slave
     * else reserved
     **/
     unsigned short specificSource;
    /*! Port number where the condition occurred
     **/
     unsigned char portNum;
    /*! Channel number where the condition occurred
     **/
     unsigned char chNum;
    /*! type of the diagnostic condition
     **/
     diag_type_t diagType;
    /*! relevance of the diagnostic condition
     **/
     diag_relevance_t diagRelevance;
    /*! diagnostic code of the condition
     **/
     unsigned short diagCode;
    /*! additional information describing the diagnostic incident
     **/
     unsigned char diagParamArray[2];
} diag_info_t;

/*! The enumeration contains the possible operation modes of the AS-i 5 parameter
 * server.
 **/
typedef enum ps_mode_e_tag
{
    /*! In the disabled mode is no recovery and backup of AS-i 5 slaves possible.
     * The disabled mode is active when the AS-i 5 operating mode is not in AS-i 5
     * protected mode or when the user disables the parameter server in the AS-i 5
     * protected mode.
     **/
	PS_DISABLED = 0,
    /*! In the freeze mode the parameter server forbid modification parameter
     * images by the AS-i 5 slaves. The AS-i 5 slaves shall forbid modification of
     * the parameter image in the AS-i 5 device. The freeze mode shall be the
     * factory default ASi5PSMode value.
     **/
	PS_FREEZE = 1,
    /*! In the sync mode the parameter server uploads a parameter image for each
     * PSUploadRequest event from an AS-i 5 slave or stores modifications by
     * parameter image update on master side.
     **/
	PS_SYNC = 2
} ps_mode_e;

/*! Generic Slave Commands for an AS-i 5 Slave
 **/
typedef enum slave_commands_t_tag
{
    /*! This command resets the AS-i 5 slave. If the slave is in middle of bus
     * communication then it losses the bus-address.
     **/
	SCMD_RESET = 0x01,
    /*! This command resets the AS-i 5 slave and restores the factory default
     * values.
     **/
	SCMD_FACTORY_RESET = 0x02,
    /*! This command disconnects the AS-i 5 Slave from the AS-i Line logical
     **/
	SCMD_DISCONNECT = 0x03,
    /*! The set_asi_configuration is used for modification of the logical address,
     * AS-i 5 slave priority configuration and actual used profile. The AS-i 5
     * slave services this command in inactive and active phase. After storing the
     * configuration the AS-i 5 slave resets.
     **/
	SCMD_SET_ASI_CONFIGURATION = 0x04,
    /*! This slave command is used to set data to an index of an AS - i 5 slave.
     **/
	SCMD_PUT_SLV_IDX = 0x05,
    /*! This slave command is used to retrieve data from an index of an AS - i 5
     * slave.
     **/
	SCMD_GET_SLV_IDX = 0x06,
    /*! This command will be used to read the device designator out of the
     * parameter image. The device designator is a UTF- 8 string with a
     * termination by a NUL character.
     **/
	SCMD_GET_DEVICE_DESIGNATOR = 0x09,
    /*! This command sets the device designator in the parameter image.
     **/
	SCMD_SET_DEVICE_DESIGNATOR = 0x0A,
    /*! This command sets the severity level of the AS-i 5 Slave. The severity
     * level determines which items of Diagnostic Information are transferred to
     * the AS-i 5 Master.
     **/
	SCMD_SET_SEVERITY_LEVEL = 0x0B,
    /*! This ODC request is used to trigger a download of the stored parameter
     * image from the parameter.
     **/
	SCMD_TRIGGER_PARAM_DOWNLOAD = 0x0C,
    /*! This command sets parameter image data into temporary parameter image.
     **/
	SCMD_SET_PARAMETER = 0x0D,
    /*! This command will be used to read parts of the parameter image.
     **/
	SCMD_GET_PARAMETER = 0x0E,
    /*! This command will be used to check AS-i 5 the parameter image change
     * indicator (PICI) of the AS-i 5 slave.
     **/
	SCMD_CHECK_PICI = 0x10,
    /*! This command will be used to replace the used PI by the temporary parameter
     * image or drop the temporary parameter image by the Parameter server, see
     * chapter 5.2.2.8 Parameter Server.
     **/
	SCMD_ASSIGN_PI = 0x11,
    /*! This command is used to check if the last operation (e.g. store NV of
     * assignPI) is still proceeding.
     **/
	SCMD_LAST_ACTION_POLLING = 0x12,
    /*! This command is used to request the current AsiTime of the Slave
     **/
	SCMD_GET_ASI_TIME = 0x13,
    /*! This command is used to set the timeout for Complex Slave communication.
     **/
	SCMD_SET_CS_TIMEOUT = 0x14,
    /*! This command is used to overwrite the substitute values stored in the
     * profile.
     **/
	SCMD_SET_SUBSTITUTE_VALUES = 0x15,
    /*! This command is used to transfer the response of a request to the Host
     * interface that was previously transferred to the master using the ODC
     * request get_service_device_cmd.
     **/
	SCMD_PUT_SERVICE_DEVICE_RSP = 0x20,
    /*! This command is used to get the requested command from the slave Service
     * Device Handler to the master Service Device Handler. The data structure of
     * the Host interface command shall be the same format that is used by the
     * command router.
     **/
	SCMD_GET_SERVICE_DEVICE_CMD = 0x21,
    /*! This command is used by the Master Service Device Handler to signalize to
     * the Slave Service Device Handler that the execution of a host command is in
     * progress.
     **/
	SCMD_SDH_HEARTBEAT = 0x22,
    /*! This command is used by the external MCU of the complex slave to signal
     * that it is ready.
     **/
	SCMD_EXT_MCU_READY = 0x23,
    /*! The reset system key data command is used by the Host to reset specific
     * system key data reference groups.
     **/
	SCMD_RST_SYS_KEY_DATA = 0x31,
    /*! Get the Safe-Bus addresses for all 4 TCs from a Slave.
     **/
	SCMD_GET_SAFE_ADDR = 0x40,
    /*! Set the Safe-Bus addresses for all 4 TCs to a Slave.
     **/
	SCMD_SET_SAFE_ADDR = 0x41,
    /*! Get the SCT profile for all 4 TCs.
     **/
	SCMD_GET_SCT_LIST = 0x42,
    /*! Get the CRC32 that the Slave calculated for it's SID.
     **/
	SCMD_GET_SID_CRC = 0x43,
    /*! The Flash Erase Command is used for Flash Access
     **/
	SCMD_FLASH_ERASE = 0x60,
    /*! The Flash Write Command is used for Flash Access
     **/
	SCMD_FLASH_WRITE = 0x61,
    /*! The Flash Read Command is used for Flash Access
     **/
	SCMD_FLASH_READ = 0x62,
    /*! This command sets the masked parameter image data into temporary parameter
     * image.
     **/
	SCMD_SET_PARAMETER_MASKED = 0x63,
    /*! This command is used to get large data from an AS-i 5 Slave by use of a
     * segmented and controlled transfer mechanism.
     **/
	SCMD_GET_STREAM = 0x64,
    /*! This command is used to write large data to an AS-i 5 Slave by use of a
     * segmented and controlled transfer mechanism.
     **/
	SCMD_SET_STREAM = 0x65,
    /*! This command is used to assign an event to the slave execution control.
     **/
	SCMD_TRIGGER_EVENT = 0xF4,
    /*! This command is used to execute a host command on the AS - i 5 master Host
     * interface by an AS- i 5 slave.
     **/
	SCMD_EXEC_HOST_CMD = 0xF5,
    /*! This command is used to execute a host command on the AS-i 5 master Host
     * interface by an AS- i 5 slave.
     **/
	SCMD_GET_HOST_CMD_RSP = 0xF6,
    /*! This command handles diagnostics from the Slave Applications. It manages
     * the diagnostics status. This means it either adds or removes diagnostics
     * from the diagnostics status. On the other hand, it is also used to transfer
     * items from the AS-i 5 Slave to the AS-i 5 Master. The Diagnostic Handler
     * therefore has a buffer that works as a FIFO for the given items and stores
     * them until they are fetched from the AS-i 5 Master.
     * Entries of Diagnostic Type information are not taken in account for the
     * Diagnostic Status but are entered in the FIFO for the collection by the
     * AS-i 5 Master.
     **/
	SCMD_HANDLE_DIAG = 0xF9,
    /*! This ODC request is used for a parameter image upload request to the AS-i 5
     * master.
     **/
	SCMD_TRIGGER_BACKUP_REQUEST = 0xF3,
    /*! The get non validated pi command is used by the slave application to
     * retrieve the not yet validated PI from the parameter client, so that the
     * slave application is able to validate the PI. The result of the validation
     * is forwarded to the Parameter Client by the parameter_checked command.
     **/
	SCMD_GET_PI = 0xFA,
    /*! The parameters checked command is used by the slave application to indicate
     * that a set of parameters retrieved by get_nonvalidated_pi from the slave
     * stack is valid or is not valid. This is indicated by the isValid
     * information in the request.
     **/
	SCMD_PARAMETER_CHECKED = 0xFB,
    /*! The parameters in use command is used by the slave application to indicate
     * that a set of parameters retrived by get_parameter is now in use by the
     * slave application. If the slave application is not able to use the
     * retrieved parameters, the parameter isUsed shall be set accordingly.
     **/
	SCMD_PARAMETER_IN_USE = 0xFC,
    /*! Sets the default PI in the Parameter Client
     **/
	SCMD_SET_DEFAULT_PI = 0xFD,
    /*! Slave command error is returned when the execution of a slave command in
     * the AS-i 5 slave fails.
     **/
	SCMD_ERROR = 0xFF
} slave_commands_t;

/*! List of protected AS-i 5 odc indexes
 **/
typedef enum odc_index_labels_tag
{
    /*! Status index of slave
     **/
	IDX_SLAVE_STATUS = 0x0000,
    /*! This represents the index of manufacturer device name
     **/
	IDX_MANUFACTURERDEVICENAME = 0x0001,
    /*! Provides the process data watchdog value.
     **/
	IDX_PROCESSDATAWATCHDOG = 0x0002,
    /*! Represents the index id of ASIC version (includes AFE and MDS)
     **/
	IDX_ASIC_VERSION = 0x0003,
    /*! This index id represents the SW-Version of CPU in ASIC
     **/
	IDX_ASIC_SW_VERSION = 0x0004,
    /*! Represents the index id for HW-Version of AS-i 5 device index
     **/
	IDX_DEVICE_HW_VERSION = 0x0005,
    /*! This index id represents the SW-Version of external CPU
     **/
	IDX_EXT_SW_VERSION = 0x0006,
    /*! AS-i 5 id of slave
     **/
	IDX_ASIID = 0x0007,
    /*! Actual used communication protocol version
     **/
	IDX_COMMUNICATION_ID = 0x0008,
    /*! Represents the index id for the manufacturer specific version of all
     * features supported by this AS-i 5 device.
     **/
	IDX_MANUFACTURER_DEVICE_VERSION = 0x0009,
    /*! ODC timeout of the actual profile
     **/
	IDX_ODC_TIMEOUT = 0x000A,
    /*! Device designator
     **/
	IDX_DEV_DESIGNATOR = 0x000D,
    /*! This represents the index of manufacturer device family
     **/
	IDX_MANUFACTURERDEVICEFAMILY = 0x000F,
    /*! Logical address of slave
     **/
	IDX_LOGICAL_ADDRESS = 0x0011,
    /*! actual bus address of slave
     **/
	IDX_BUS_ADDRESS = 0x0012,
    /*! AS-i 5 slave profile id for the actual used device profile
     **/
	IDX_ACUTAL_ASI_PROFILE_ID = 0x001A,
    /*! List of all supported AS-i 5 device profiles
     **/
	IDX_ASI_DEVICE_PROFILE_LIST = 0x001B,
    /*! Required count of transport channel
     **/
	IDX_NUMBER_USED_TC = 0x0020,
    /*! TLSC information
     **/
	IDX_TLSC_LIST = 0x0021,
    /*! Index number for the tc carrier assignment list.
     **/
	IDX_TC_CARRIER_ASSIGN_LIST = 0x0022,
    /*! The index id for the AFE Gain information of the AS-i slave
     **/
	IDX_GAIN = 0x0023,
    /*! Represents index of energy saving group.
     **/
	IDX_ENERGY_SAVING_GROUP = 0x0030,
    /*!
     **/
	IDX_SKD_SNR_P0 = 0x0060,
    /*!
     **/
	IDX_SKD_SNR_P1 = 0x0061,
    /*!
     **/
	IDX_SKD_SNR_P2 = 0x0062,
    /*!
     **/
	IDX_SKD_RSSI_P0 = 0x0064,
    /*!
     **/
	IDX_SKD_RSSI_P1 = 0x0065,
    /*!
     **/
	IDX_SKD_RSSI_P2 = 0x0066,
    /*!
     **/
	IDX_SKD_METRIC_P0 = 0x0068,
    /*!
     **/
	IDX_SKD_METRIC_P1 = 0x0069,
    /*!
     **/
	IDX_SKD_METRIC_P2 = 0x006A,
    /*!
     **/
	IDX_SKD_SNR_NOK_CNT_P0 = 0x006C,
    /*!
     **/
	IDX_SKD_SNR_NOK_CNT_P1 = 0x006D,
    /*!
     **/
	IDX_SKD_SNR_NOK_CNT_P2 = 0x006E,
    /*!
     **/
	IDX_SKD_METRIC_NOK_CNT_P0 = 0x0070,
    /*!
     **/
	IDX_SKD_METRIC_NOK_CNT_P1 = 0x0071,
    /*!
     **/
	IDX_SKD_METRIC_NOK_CNT_P2 = 0x0072,
    /*!
     **/
	IDX_SKD_CRC_NOK_CNT_P0 = 0x0074,
    /*!
     **/
	IDX_SKD_CRC_NOK_CNT_P1 = 0x0075,
    /*!
     **/
	IDX_SKD_CRC_NOK_CNT_P2 = 0x0076,
    /*!
     **/
	IDX_SKD_VALID_NOK_CNT_P0 = 0x0078,
    /*!
     **/
	IDX_SKD_VALID_NOK_CNT_P1 = 0x0079,
    /*!
     **/
	IDX_SKD_VALID_NOK_CNT_P2 = 0x007A,
    /*!
     **/
	IDX_SKD_CRC_CORR_CNT_P0 = 0x007C,
    /*!
     **/
	IDX_SKD_CRC_CORR_CNT_P1 = 0x007D,
    /*!
     **/
	IDX_SKD_CRC_CORR_CNT_P2 = 0x007E,
    /*!
     **/
	IDX_SKD_TC_NOK_CNT_P0 = 0x0080,
    /*!
     **/
	IDX_SKD_AMC_REDUNDANCY = 0x0082,
    /*! List of unavailable carriers
     **/
	IDX_LIST_UNAVAILABLE_CARRIERS = 0x0084,
    /*! List of all PDTs
     **/
	IDX_PDT_LIST = 0x0100,
    /*! Required threshold for SNR quality data
     **/
	IDX_SNR_LIMIT = 0x0110,
    /*! SNR quality data
     **/
	IDX_SNR_QUALITY = 0x0111,
    /*! number of active entries in the diagnostic status
     **/
	IDX_NUM_OF_DIAG_ENTRIES = 0x0120,
    /*! maximum supported size of the diagnostic status
     **/
	IDX_MAX_NUM_OF_DIAG_ENTRIES = 0x0121,
    /*! severity level
     **/
	IDX_SEVERITY_LVL = 0x0122,
    /*! diagnostic status list
     **/
	IDX_DIAG_STATUS_LIST = 0x0123,
    /*! diagnostic location config
     **/
	IDX_DIAG_LOC_CFG = 0x0124,
    /*! Version of the parameter backup system
     **/
	IDX_PARAM_BACKUP_SYS_VER = 0x0150,
    /*! Parameter backup request
     **/
	IDX_HAS_PARAM_BACKUP_REQ = 0x0151,
    /*! Status of the Parameter Client
     **/
	IDX_PC_STATUS = 0x0153,
    /*! Parameter image change indicator
     **/
	IDX_PICI = 0x0155,
    /*! ASIID of last backup master
     **/
	IDX_ASIID_LAST_BACKUP_MA = 0x0156,
    /*! actual assid of the connected master
     **/
	IDX_CURRENT_MASTER_ASIID = 0x0157,
    /*! Identifies the index for numbers of port on AS-i device.
     **/
	IDX_NUM_PORTS = 0x0010,
    /*! Identifies the index for the slave process data input image.
     **/
	IDX_SCDH_PDI = 0x0031,
    /*! Identifies the index for the slave process data output image.
     **/
	IDX_SCDH_PDO = 0x0032,
    /*! Identifies the index for the last configuration error code.
     **/
	IDX_LAST_CONFIG_ERROR = 0x0159
} odc_index_labels;

/*!
 **/
typedef enum return_t_tag
{
    /*! Execution successful
     **/
	RET_OK = 0,
    /*! Execution failed
     **/
	RET_FAIL = 1,
    /*! Out of range access.
     **/
	RET_OUTOFRANGE = 2,
    /*! Invalid instance id supplied
     **/
	RET_INVALID_ASI5_INSTANCE_ID = 3,
    /*! Requested index number is invalid
     **/
	RET_INVALID_SLAVE_INDEX = 4,
    /*! Supplied Index is read only
     **/
	RET_READ_ONLY_SLAVE_INDEX = 5,
    /*! Requested port number is invalid
     **/
	RET_INVALID_PORT = 6,
    /*! Supplied data is invalid for given index
     **/
	RET_INVALID_SLAVE_INDEX_DATA = 7,
    /*! Overflow of Diagnostic Status: Maximum supported number of active
     * Diagnostic Conditions at a time reached.
     **/
	RET_DH_DIAG_STATUS_OVRFLW = 8,
    /*! Buffer for unsent diagnostic data is not empty: The last set Diagnostic
     * Condition has not been collected from the AS-i 5 Master yet.
     **/
	RET_DH_UNSENT_DIAG_DATA_PRESENT = 9,
    /*! An identical rising does already exist in the diagnostic status.
     * Nevertheless has the message been sent to the Diagnostic Sever.
     **/
	RET_DH_DUPLICATE_ENTRY = 10,
    /*! No matching rising condition found in Diagnostic Status: Diagnostic
     * Condition can not be removed from the Diagnostic Status
     **/
	RET_DH_NO_RISING_CONDITION = 11,
    /*! Invalid Diagnostic Type is: Diagnostic Type of the Diagnostic Condition is
     * not supported.
     **/
	RET_DH_INVALID_DIAG_TYPE = 12,
    /*! Invalid severity: Given severity level is not supported.
     **/
	RET_DH_INVALID_SEVERITY = 13,
    /*! Combination of Port and Channel number can not be combined inside the
     * Diagnostic Location according to the Diagnostic Location Config
     **/
	RET_DH_DIAG_LOCATION_ERR = 14,
    /*! Diagnostic Location Config invalid
     **/
	RET_DH_DIAG_LOCATION_CFG_INV = 15,
    /*! DEPRECATED
     **/
	RET_DH_PARAM_NULLPOINTER = 16,
    /*! Invalid instance id supplied
     **/
	RET_MAC_INVALID_INSTANCE_ID = 17,
    /*! Invalid instance number supplied
     **/
	RET_MAC_INVALID_INSTANCE_NO = 18,
    /*! Invalid subcycle supplied
     **/
	RET_MAC_SUBCYCLE_OUTOFRANGE = 19,
    /*! Invalid carriertable supplied
     **/
	RET_MAC_CARRIERTABLE_NULLPTR = 20,
    /*! Invalid carrierevaluation
     **/
	RET_MAC_AMC_CARRIEREVAL_INV = 21,
    /*! Valid carrierevaluation
     **/
	RET_MAC_AMC_CARRIEREVAL_VALID = 22,
    /*! Invalid parameter index
     **/
	RET_PC_INV_PARAM_IDX = 23,
    /*! Given PI directive is invalid
     **/
	RET_PC_INV_PI_DIRECTIVE = 24,
    /*! Invalid parameter size
     **/
	RET_PC_INV_PI_SIZE = 25,
    /*! DEPRECATED
     **/
	RET_PC_INV_DESIGNATOR_SIZE = 26,
    /*! DEPRECATED
     **/
	RET_PC_EVENT_PENDING = 27,
    /*! Modification of PI is not allowed
     **/
	RET_PC_MOD_OF_PI_NOT_ALLOWED = 28,
    /*! Supplied PI is invalid
     **/
	RET_PC_PI_INVALID = 29,
    /*! No temporary PI available that can be set
     **/
	RET_PC_NO_TEMP_PI = 30,
    /*! Priority out of range.
     **/
	RET_ODCSCH_PRIO_OUT_OF_RANGE = 31,
    /*! The ODC or PODC request is not valid.
     **/
	RET_ODCSCH_REQUEST_INVALID = 32,
    /*! The ODC or PODC request could not be sent the AS-i 5 slave because this
     * logical address is not available.
     **/
	RET_ODCSCH_SLV_NOT_IN_LDS = 33,
    /*! The ODC or PODC request could not be sent the AS-i 5 slave because this
     * logical address is multiple assigned in the AS-i 5 System.
     **/
	RET_ODCSCH_MULTI_SLV_IN_LDA = 34,
    /*! The ODC request could not be sent the AS-i 5 slave because the AS-i 5 Slave
     * does not support the requested Port number.
     **/
	RET_ODCSCH_PORT_NOT_AVAIL = 35,
    /*! The ODC or PODC request could not be sent the AS-i 5 slave because this
     * channel is not available.
     **/
	RET_ODCSCH_SLV_NOT_AVAIL = 36,
    /*!
     **/
	RET_ODCSCH_QUEUE_TIMEOUT = 37,
    /*! No event pending
     **/
	RET_EVT_CLBK_NO_EVENT_PENDING = 38,
    /*! error in response decoding of slave
     **/
	RET_EVT_INV_SLAVE_RESPONSE = 39,
    /*! DEPRECATED
     **/
	RET_ODCMAN_RESP_OK = 40,
    /*! DEPRECATED
     **/
	RET_ODCMAN_RESP_FAIL = 41,
    /*! Port has pending request
     **/
	RET_ODCMAN_PORT_FREE = 42,
    /*! ODC command is not processed yet
     **/
	RET_ODCMAN_CMD_IN_QUEUE = 43,
    /*! ODC command is not processed yet
     **/
	RET_ODCMAN_RESP_NOT_RECEIVED = 44,
    /*! one of the values in the provided AS-i 5 slave configuration is not valid
     **/
	RET_MU_ASI_CFG_INVALID = 45,
    /*! ASi configuration can not be updated because slave is storing to nv
     **/
	RET_MU_ASI_CFG_STORING_IN_PROGR = 46,
    /*! This error is thrown if the user tries to update AS-i config in active
     * communication
     **/
	RET_MU_ASI_CFG_ACTIVE_COM = 47,
    /*! No more requests can be queued as queue is full
     **/
	RET_ODCMAN_QUEUE_FULL = 48,
    /*! Queue is empty
     **/
	RET_ODCMAN_QUEUE_EMPTY = 49,
    /*! The requested command does not exist
     **/
	RET_ODCMAN_NO_CMD_EXIST = 50,
    /*! The process data exchange is stopped.
     **/
	RET_CDH_PROCESS_STOP = 51,
    /*! Command router is busy with maximum number of request under process
     **/
	RET_CMDROUTER_BUSY = 52,
    /*! Invalid host command size
     **/
	RET_SLV_SDH_INV_HOST_CMD_SIZE = 53,
    /*! Another host command execution is already in progress
     **/
	RET_SLV_SDH_REQ_IN_PROGRESS = 54,
    /*! An internal communication error occurred. The execution of the host command
     * has failed.
     **/
	RET_SLV_SDH_COM_ERROR = 55,
    /*! Invalid diagnostic location config supplied
     **/
	RET_DS_DIAG_LOG_CFG_INV = 56,
    /*! Invalid logical address supplied
     **/
	RET_DS_LOG_ADR_INV = 57,
    /*! Invalid diagnostic information supplied (global source, specific source,
     * diagnostic type, diagnostic relevance)
     **/
	RET_DS_DIAG_INFO_INV = 58,
    /*! Invalid diagnostic event data supplied
     **/
	RET_DS_EVT_DATA_INV = 59,
    /*! Invalid diagnostic instance supplied
     **/
	RET_DS_INSTANCE_INV = 60,
    /*! Invalid global source supplied
     **/
	RET_DS_GLOBAL_SRC_INV = 61,
    /*! Invalid specific source supplied
     **/
	RET_DS_SPEC_SRC_INV = 62,
    /*! Invalid severity supplied
     **/
	RET_DS_SEVERITY_INV = 63,
    /*! Invalid logical address supplied
     **/
	RET_MA_SDH_LOG_ADR_INV = 64,
    /*! Supplied Diagnostic Code is invalid. For the transfer between AS-i 5 Slave
     * and AS-i 5 Master the maximum value is 1023.
     **/
	RET_DH_DIAG_CODE_INV = 65,
    /*! Diagnostic Relevance invalid
     **/
	RET_DH_DIAG_RELEVANCE_INV = 66,
    /*! The event manager has no waiting event which is handled.
     **/
	RET_EM_NO_WAITING_EVENT = 67,
    /*! This return value is returned when the memory of slave is not present.
     **/
	RET_ENV_OUT_OF_MEMORY = 68,
    /*! Signals that storing or reading of NV is not possible because the NV is
     * busy.
     **/
	RET_ENV_NV_BUSY = 69,
    /*! not used any longer - do not delete this could cause problems on SOAP side
     **/
	RET_MAMC_ORDS_ILLEGAL_TOUT = 70,
    /*! No AMC ORDS request pending
     **/
	RET_MAMC_ORDS_NO_REQ_PENDING = 71,
    /*! No AMC ORDS response expected. There was no previous request.
     **/
	RET_MAMC_ORDS_NO_RSP_EXPECTED = 72,
    /*! not used any longer - do not delete this could cause problems on SOAP side
     **/
	RET_MAMC_ORDS_MORE = 73,
    /*! ILLEGAL not used any longer
     **/
	RET_MAMC_ORDS_NO_RSP_PENDING = 74,
    /*! To much retries in AMC ORDS sequence.
     **/
	RET_MAMC_ORDS_ERR_RETRIES = 75,
    /*! Slave didn't responsed to AMC ORDS request during timeout.
     **/
	RET_MAMC_ORDS_ERR_TIMEOUT = 76,
    /*! The AMC ORDS sequence finished. But the ORDS state machine didn't reached
     * ok or error state.
     **/
	RET_MAMC_ORDS_ERR_NO_RESPONSE = 77,
    /*! The provided odc caller is invalid.
     **/
	RET_PH_INVALID_ODC_CALLER = 78,
    /*! There is no request present.
     **/
	RET_PH_NO_REQ_PENDING = 79,
    /*! Another request is currently in progress.
     **/
	RET_PH_REQ_IN_PROGRESS = 80,
    /*! The provided parameter image is invalid.
     **/
	RET_PH_INVALID_PI = 81,
    /*! Another request is already in progress.
     **/
	RET_PC_REQ_IN_PROGRESS = 82,
    /*! There is no request present.
     **/
	RET_PC_NO_REQ_PENDING = 83,
    /*! DEPRECATED
     **/
	RET_PC_BUSY = 84,
    /*! There is no request present.
     **/
	RET_CS_NO_REQ_PENDING = 85,
    /*! A request is currently processed by the Complex Slave. Poll the function
     * again.
     **/
	RET_CS_BUSY = 86,
    /*! During an AMC ORDS request the state machine wasn't in idle state.
     **/
	RET_MAMC_ORDS_ERR_NOT_IN_IDLE = 87,
    /*! The response from the slave will not fit into the response buffer
     **/
	RET_MAMC_ORDS_ERR_RSP_BUFFER = 88,
    /*! PDT Configuration is invalid.
     **/
	RET_CDH_INVALID_PDTC = 89,
    /*! TC Configuration is invalid.
     **/
	RET_CDH_INVALID_TCC = 90,
    /*! PDT Configuration and TC Configuration do not fit together.
     **/
	RET_CDH_PDTC_TCC_MISSMATCH = 91,
    /*! Invalid subcycle is requested.
     **/
	RET_CDH_INVALID_SUBCYCLE_SETUP = 92,
    /*! Invalid subcycle is requested.
     **/
	RET_CDH_INVALID_SUBCYCLE = 93,
    /*! Safety-App tried to configure a none-safety slave in the RTI.
     **/
	RET_CDH_NONE_SAFETY_IN_RTI = 94,
    /*! Safety-App tried to configure invalid number of TCs.
     **/
	RET_CDH_INVALID_TC_MASK = 95,
    /*! Safety-App tried to configure an invalid FTPI index.
     **/
	RET_CDH_INVALID_FTPI_INDEX = 96,
    /*! invalid FTPIin or FTPIout Pointer configured
     **/
	RET_CDH_INVALID_FTPI_PTR = 97,
    /*! invalid RTIin or RTIout Pointer configured
     **/
	RET_CDH_INVALID_RTI_PTR = 98,
    /*! slave aborted AMC ORDS request
     **/
	RET_MAMC_ORDS_ABORTED = 99,
    /*! Unexpected response from slave
     **/
	RET_MAMC_ORDS_UNEXPECTED = 100,
    /*! invalid Arbitration Mask
     **/
	RET_CDH_INVALID_AMASK = 101,
    /*! The assigned logical address is out of range.
     **/
	RET_MU_ASI_CFG_LOGADDR_OUT_RANG = 102,
    /*! OAMCors_ORDSrequest called with invalid parameter
     **/
	RET_MAMC_ORDS_INV_PARAM = 103,
    /*! spuriois wakeup from condition variable
     **/
	RET_MAMC_ORDS_CONDVAR_ERR = 104,
    /*! Slave has logAddr 0.
     **/
	RET_CDH_LOGADDR_0 = 105,
    /*! The mutex could not be acquired because it was already locked.
     **/
	RET_ENV_MUTEX_BUSY = 106,
    /*! This return code is triggered when no event is ready to deliver over AMC.
     **/
	RET_EHL_NO_PENDING_EVENT = 107,
    /*! This return code is triggered when a command is invoked with the wrong
     * access level of command router
     **/
	RET_CR_INCORRECT_ACCESS_LEVEL = 108,
    /*! This error code is triggered when the segmentation had a communication
     * error.
     **/
	RET_TC_ORDS_COM_ERROR = 109,
    /*! This return code is triggered when the communication is timeouted.
     **/
	RET_TC_ORDS_COM_TIMEOUT = 110,
    /*! This identifies a unknown source from Segmented data.
     **/
	RET_TC_ORDS_INVALID_CALLER = 111,
    /*! This error is returned if the caller_id_t is not correct
     **/
	RET_INVALID_CALLER_ID = 112,
    /*! An ORDS request is aborted because the ODC timeout of the slave is 0.
     **/
	RET_MAMC_ORDS_ODCTOUT_INVALID = 113,
    /*! More then one Slave is configured to this LogAddr. The first found Slave is
     * returned.
     **/
	RET_CM_DUPLICATE_LOGADDR = 114,
    /*! Invalid LogAddr.
     **/
	RET_CM_INVALID_LOGADDR = 115,
    /*! LogAddr not found.
     **/
	RET_CM_UNKNOWN_LOGADDR = 116,
    /*! BusAddr not found.
     **/
	RET_CM_UNUSED_BUSADDR = 117,
    /*! Invalid PCD Instance selected.
     **/
	RET_CM_INVALID_PCD_INSTANCE = 118,
    /*! Carrier configuration is inconsistent.
     **/
	RET_CM_CARRIER_INCONSISTENT = 119,
    /*! TC configuration is inconsistent.
     **/
	RET_CM_TC_INCONSISTENT = 120,
    /*! PCD can not be stored. Memory overflow.
     **/
	RET_CM_PCD_OVRFLW = 121,
    /*! Master is not in Configuration Mode.
     **/
	RET_CM_NO_CFG_MODE = 122,
    /*! LogAddr 0 found.
     **/
	RET_CM_LOGADDR_0 = 123,
    /*! LMS not empty.
     **/
	RET_CM_LMS_NOT_EMPTY = 124,
    /*! Management Call wasn't be handled within a specific time.
     **/
	RET_CM_MANAGEMENT_TIMEOUT = 125,
    /*! Management Call is in current state not possible.
     **/
	RET_CM_MANAGEMENT_NOT_POSSIBLE = 126,
    /*! Error during Management Call.
     **/
	RET_CM_MANAGEMENT_ERROR = 127,
    /*! No response for this Management Call.
     **/
	RET_CM_MANAGEMENT_NO_RESPONSE = 128,
    /*! Trigger Evaluate in Main-loop. (This is no return code. It's for internal
     * use of the Configuration Manager.)
     **/
	RET_CM_TRIGGER_EVAL_ALL = 129,
    /*! Destination LogAdd of ChangeLogAddr Request is already in use.
     **/
	RET_CM_LOGADDR_INUSE = 130,
    /*! Process Data Image would overlap after ChangeLogAddr Request.
     **/
	RET_CM_PDI_OVERLAPPING = 131,
    /*! ASIID not found in LDS.
     **/
	RET_CM_UNKNOWN_ASIID = 132,
    /*! Invalid System Key Data type.
     **/
	RET_CM_INVALID_SKD_TYPE = 133,
    /*! Vendor ID of vendor specific profile doesn't match.
     **/
	RET_CM_WRONG_VENDORID = 134,
    /*! Configuration RW-Lock is busy. (This is no return code. It's for internal
     * use of the Configuration Manager.)
     **/
	RET_CM_BUSY = 135,
    /*! Slave can not be allocated because not enough TCs are available.
     **/
	RET_CARRM_TC_OVRFLW = 136,
    /*! Slave can not be allocated because not enough carriers are available.
     **/
	RET_CARRM_CARRIER_OVRFLW = 137,
    /*! NV data not successful stored
     **/
	RET_SLV_ENV_STORE_NV_DATA_ERR = 138,
    /*! NV data not successful read
     **/
	RET_SLV_ENV_GET_NV_DATA_ERR = 139,
    /*! no active page found in NVRAM
     **/
	RET_SLV_ENV_NV_DATA_EMPTY = 140,
    /*! unable to initialize slave stack
     **/
	RET_SLV_ENV_INIT_ERROR = 141,
    /*! access to illegal page in NVRAM
     **/
	RET_SLV_ENV_NV_ILLEGAL_PAGE = 142,
    /*! illegal address or length in NVRAM access
     **/
	RET_SLV_ENV_NV_ADR_LENGTH_ERR = 143,
    /*! Access to illegal index number/range.
     **/
	RET_SSA_IDX_ILLEGAL_RANGE = 144,
    /*! Simple slave application not configured or configured wrong.
     **/
	RET_SSA_NOT_CONFIGURED = 145,
    /*! EEPROM read or write operation failed. PCD may be inconsistent.
     **/
	RET_CM_EEPROM_ERROR = 146,
    /*! Slave Cyclic Data Handler is not configured or configured wrong.
     **/
	RET_SCDH_CONFIG_ERROR = 147,
    /*! EEPROM was corrupt. Wrote default values to it.
     **/
	RET_EEPROM_CORRUPT = 148,
    /*! CarrMngr_EvaluateSingleSlave() failed and the Configuration Manager should
     * do a subcycle change request.
     **/
	RET_SUBCYCLE_CHANGE_REQUEST = 149,
    /*! Checksum error on Service Device Interface UART
     **/
	RET_SLV_ENV_UART_CHK_ERR = 150,
    /*! Service Device Interface UART inter-frame timeout
     **/
	RET_SLV_ENV_UART_TIMEOUT = 151,
    /*! This command for SubcycleChangeRequest can not be executed in this state.
     * Retry later or reset the blocking slave.
     **/
	RET_CDH_SUBCYCLECHANGE_BUSY = 152,
    /*! Signals that ParameterServer is frozen
     **/
	RET_PS_IN_FREEZE_MODE = 153,
    /*! requested slave is not found in the PI set
     **/
	RET_PS_SLV_NOT_IN_PI_SET = 154,
    /*! The upload request to the PS failed
     **/
	RET_PS_UPLOAD_REQ_FAILED = 155,
    /*! the send request to the PS failed
     **/
	RET_PS_SEND_REQ_FAILED = 156,
    /*! signals that the PI didn't get saved into NV memory
     **/
	RET_PS_PI_SAVE_FAILED = 157,
    /*! signals if the index or the count were invalid
     **/
	RET_PS_INV_INDX_OR_COUNT = 158,
    /*! signals if the PS is a PI set changed due to a configuration instance
     * change
     **/
	RET_PS_IN_PI_SET_CHANGE = 159,
    /*! signals that the communication with a slave was faulty
     **/
	RET_PS_SLV_COM_ERROR = 160,
    /*! The request length is not in the valid range.
     **/
	RET_CR_INV_REQ_LEN = 161,
    /*! Parameter image is inconsistent (slave is in ASI5LIS). Request can not be
     * executed.
     **/
	RET_PS_INCONSISTENT_SLAVES = 162,
    /*! signals that the command was rejected because the Configuration Manager
     * didn't disclose his operation mode and configuration instance yet.
     **/
	RET_PS_NO_CM_OPMODE = 163,
    /*!
     **/
	RET_ODCMAN_DATA_INVALID_LENGTH = 164,
    /*! LogAddr is not valid / out of range
     **/
	RET_CDH_LOGADDR_OUTOFRANGE = 165,
    /*! Device designator is too long or not terminated by zero.
     **/
	RET_CM_DEV_DESIG_TOO_LONG = 166,
    /*! Device designator is not unique.
     **/
	RET_CM_DEV_DESIG_NOT_UNIQUE = 167,
    /*! Error during auto set logAddr.
     **/
	RET_CM_AUTO_LOGADDR_FAILED = 168,
    /*! The KeyDataID is unknown.
     **/
	RET_CR_INVALID_KEYDATAID = 169,
    /*! The offset value is not in the valid range.
     **/
	RET_CR_INVALID_OFFSET = 170,
    /*! The command is unknown.
     **/
	RET_CR_INVALID_COMMAND = 171,
    /*! device designator not found
     **/
	RET_CM_UNKNOWN_DEV_DESIG = 172,
    /*! can not calculate process data length because PDT configuration is not
     * valid.
     **/
	RET_CM_INVALID_PDTC = 173,
    /*! Slave is not in ASI5LPS.
     **/
	RET_PS_SLV_NOT_PROJECTED = 174,
    /*! Process data overlapping
     **/
	RET_CDH_PD_OVERLAPPING = 175,
    /*! logAddr not found in Parameter set and no free memory slot left.
     **/
	RET_PS_NO_OPEN_SLOT = 176,
    /*! Invalid Diagnostic Type: Diagnostic Type of the Diagnostic Condition is not
     * supported.
     **/
	RET_DIAG_INV_TYPE = 177,
    /*! Invalid Diagnostic Relevance: Diagnostic Relevance of the Diagnostic
     * Condition is not supported.
     **/
	RET_DIAG_INV_RELEVANCE = 178,
    /*! Invalid Combination of Diagnostic Type and Relevance.
     **/
	RET_DIAG_INV_TYPE_RELEVANCE_CMB = 179,
    /*! The Event FIFO has overflown.
     **/
	RET_DS_FIFO_OVERFLOW = 180,
    /*! Slave didn't accept configuration
     **/
	RET_CM_SLV_SET_ERROR = 181,
    /*! Parameter image download was unsuccessful. (e.g. because of negative store
     * Permission)
     **/
	RET_PS_DOWNLOAD_USUCCESSFUL = 182,
    /*! general pthread error
     **/
	RET_PTHREAD_ERROR = 183,
    /*! Slave returned an error response on ODC request sent by the Master Service
     * Device Handler.
     **/
	RET_MA_SDH_SLV_CMD_ERROR = 184,
    /*! The returned host command size via ODC does not match the announced size by
     * the event.
     **/
	RET_MA_SDH_HOST_CMD_SIZE_ERR = 185,
    /*! PS returns this code when during the examination of the LPS there was at
     * least one Slave that was configured not active in the LPS
     **/
	RET_PS_SLV_NOT_ACTIVE = 186,
    /*! The number of requested diag items is too high.
     **/
	RET_CR_INV_NUM_ITEMS_REQUSTED = 187,
    /*! The given length for the index doesn't match the expected.
     **/
	RET_INVALID_SLAVE_INDEX_LENGTH = 188,
    /*! The given combination of Diagnostic Relevance and Diagnostic Type is not
     * allowed.
     **/
	RET_DH_TYPE_RELEVANCE_COMB_INV = 189,
    /*! Error occured in Process Data Multiplexing Mechanism.
     **/
	RET_CDH_MUX_ERROR = 190,
    /*! There is no pending offset request for the given command and parameter
     * combination.
     **/
	RET_CR_INV_OFFSET_PARAM = 191,
    /*! failed to delete NVRAM
     **/
	RET_SLV_ENV_DELETE_NV_DATA_ERR = 192,
    /*! slave disconnected from AS-i line while ORDS request in progress
     **/
	RET_MAMC_ORDS_SLAVE_BUSOFF = 193,
    /*! This return code is triggered when a command is invoked by a caller which
     * is not allowed to access this command
     **/
	RET_ODCMAN_NO_ACCESS_RIGHTS = 194,
    /*! Access to illegal index number/range.
     **/
	RET_SSA_ILLEGAL_PARAM = 195,
    /*! No AS-i 5 Master is present to communicate with.
     **/
	RET_SLV_SDH_NO_MASTER = 196,
    /*! TCords_ORDSrequest called with invalid parameters
     **/
	RET_TC_ORDS_INV_PARAM = 197,
    /*! TCords_ORDSrequest called because of a reset
     **/
	RET_TC_ORDS_COM_RESET = 198,
    /*! An invalid piType has been requested.
     **/
	RET_INV_PI_TYPE = 199,
    /*! The called function can not be used at the moment because the PC is
     * processing requests from a differen caller.
     **/
	RET_PC_ALREADY_LOCKED = 200,
    /*! The request could not be completed. Call PC_LastActionPolling to proceed
     * with the processing.
     **/
	RET_PC_DO_POLLING = 201,
    /*! A timeout occurred during the execution. This might happen if the master
     * was not able to collect a changed PI in the Slave in a predefined time
     * distance.
     **/
	RET_PC_TIMEOUT = 202,
    /*! Invalid LogAddr.
     **/
	RET_PS_INVALID_LOGADDR = 203,
    /*! Error is thrown if a unlock or weaken of a lock is requested, but no lock
     * has been taken before.
     **/
	RET_PC_LOCK_ERROR = 204,
    /*! If the TC ORDS could not correct transmission errors, the calculated crc
     * does not match the received crc.
     **/
	RET_TC_ORDS_CRC_MISMATCH = 205,
    /*! TC ORDS could not find a free (cot already assigned) communication handler.
     **/
	RET_TC_ORDS_NO_FREE_COM_HDL = 206,
    /*! Wrong Flash Key in Flash Access Command.
     **/
	RET_FLASH_KEY_FAIL = 207,
    /*! Error in Flash Command Execution.
     **/
	RET_FLASH_EXECUTION_FAIL = 208,
    /*! Flash Write Protection is enabled, so a Write or Erase Command cannot be
     * executed.
     **/
	RET_FLASH_WRITE_PROTECTED = 209,
    /*! An ODC timeout has occurred during the execution of a slave command.
     **/
	RET_SLV_ODC_TIMEOUT = 210,
    /*! The requested slave command could not be executed. The affected functional
     * group is currently sanitizing its internal state. This error may happen
     * after an ODC timeout.
     **/
	RET_SLV_SANITIZING = 211,
    /*! This error occurs if a response is polled from the Slave Service Device
     * Handler without starting a requets.
     **/
	RET_SLV_SDH_NO_REQ_PENDING = 212,
    /*! Invalid configuration. The PC supports only Simple Slave with both
     * Parameter Client and Parameter Handler internal or Complex Slave with
     * Parameter Client internal and Parameter Handler external.
     **/
	RET_PC_INVALID_CFG = 213,
    /*! Profile could not be found in FactoryPage.
     **/
	RET_SLV_ENV_NV_PROFILE_FOUND = 214,
    /*! NVRAM Checksum Error
     **/
	RET_SLV_ENV_NV_CHECKSUM_ERROR = 215,
    /*! Internal error in the Parameter Client occurred.
     **/
	RET_PC_INTERNAL_ERROR = 216,
    /*! Requested index number is unknown, but may be implemented by the Complex
     * Slave.
     **/
	RET_UNKNOWN_SLAVE_INDEX = 217,
    /*! The value of OdcTimeout is invalid.
     **/
	RET_ODCMAN_ODCTOUT_INVALID = 218,
    /*! Invalid mask used to set parameters.
     **/
	RET_PC_INV_MASK = 219,
    /*! The given time is invalid
     **/
	RET_INVALID_TIME = 220,
    /*! Invalid value set in PCD or CDI.
     **/
	RET_CM_INVALID_CONFIG = 221,
    /*! Invalid transport channel.
     **/
	RET_CM_INVALID_TC = 222,
    /*! Slave has wrong ProfileID. Operation an not be proceeded.
     **/
	RET_PS_WRONG_PROFILE = 223,
    /*! AutoSetLogicalAddress didn't find a free gap to address one or more slaves.
     **/
	RET_CM_NO_FREE_GAP = 224,
    /*! There is no data available for the given offset value.
     **/
	RET_CR_OFFSET_NO_DATA = 225,
    /*! NVRAM Checksum Error
     **/
	RET_MA_ENV_NV_CHECKSUM_ERROR = 226,
    /*! An error occurred while accessing the NVRAM by the Diagnostic Server. The
     * Diagnostic History is cleared to resolve the Problem.
     **/
	RET_DS_NVRAM_ERROR = 227,
    /*! Given start index is invalid
     **/
	RET_DS_INVAID_START_INDEX = 228,
    /*! TC ORDS got an error while using a condition or a mutex variable.
     **/
	RET_TC_ORDS_COND_ERROR = 229,
    /*! Size of Device Designator is invalid
     **/
	RET_MU_INV_DESIGNATOR_SIZE = 230,
    /*! Safety-App tried to configure an invalid RTI index.
     **/
	RET_CDH_INVALID_RTI_INDEX = 231,
    /*! The provided buffer for the response is to small.
     **/
	RET_TC_ORDS_RSP_BUFFER_ERROR = 232,
    /*! The FIFO is empty.
     **/
	RET_CS_FIFO_EMPTY = 233,
    /*! The FIFO is full.
     **/
	RET_CS_FIFO_FULL = 234,
    /*! The port is already in use.
     **/
	RET_CS_PORT_IN_USE = 235,
    /*! The operation is not allowed with the current configuration.
     **/
	RET_CS_INCORRECT_MODE = 236,
    /*! The provided Complex Slave configuration is invalid.
     **/
	RET_CS_INVALID_CONFIG = 237,
    /*! The requested port is not in the valid range.
     **/
	RET_CS_INVALID_PORT = 238,
    /*! The provided CallerId is not valid for this request.
     **/
	RET_CS_INVALID_CALLER = 239,
    /*! The request length is not in the valid range.
     **/
	RET_CR_INV_RSP_LEN = 240,
    /*! A timeout has occurred during the execution of a complex slave command.
     **/
	RET_CS_TIMEOUT = 241,
    /*! The requested Slave Command is not implemented in external CPU.
     **/
	RET_CS_UNKNOWN_SLAVE_CMD = 242,
    /*! The format of the requested Slave Command is not right.
     **/
	RET_CS_INVALID_SLAVE_CMD = 243,
    /*! The length of the requested Slave Command is not right.
     **/
	RET_CS_INVALID_LENGTH = 244,
    /*! Streaming engine detected an invalid stream header byte.
     **/
	RET_STREAM_HEADER_INVALID = 245,
    /*! Streaming engine detected an unknown stream id at stream start.
     **/
	RET_STREAM_ID_UNKNOWN = 246,
    /*! Streaming engine detected invalid parameters at stream start.
     **/
	RET_STREAM_PARAMETER_INVALID = 247,
    /*! Streaming engine is in the wrong state.
     * This command can only be executed after a valid stream start.
     **/
	RET_STREAM_NOT_OPEN = 248,
    /*! Streaming engine detected a flow control error.
     **/
	RET_STREAM_FLOWCTRL_ERROR = 249,
    /*! Streaming engine detected an address or memory overflow.
     **/
	RET_STREAM_OVERFLOW = 250,
    /*! Stream CRC doesn't match.
     **/
	RET_STREAM_CRC_MISMATCH = 251,
    /*! Generic ASi-5 IO-Link gateway profile family: IO-Link master mailbox busy,
     * another request is pending
     **/
	RET_IOL_MB_BUSY = 0x1010,
    /*! Generic ASi-5 IO-Link gateway profile family: No IO-Link device connected
     * to the requested port
     **/
	RET_IOL_NO_IOL_DEVICE_CONNECTED = 0x1011,
    /*! Generic ASi-5 IO-Link gateway profile family:
     * Signalize an ErrorType (ISDU error) like described in the
     * IO-Link-Specification in Annex C.
     * This error code is followed by two additional bytes in the ODC response:
     * - the first byte is the "Error Code" of the IO-Link ErrorType
     * - the second byte is the "Additional Code" of the IO-Link ErrorType
     **/
	RET_IOL_ISDU_ERROR = 0x1012
} return_t;


#endif /* ASI5_H_ */
