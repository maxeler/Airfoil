/**\file */
#ifndef SLIC_DECLARATIONS_airfoildfe_H
#define SLIC_DECLARATIONS_airfoildfe_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define airfoildfe_nFlushPipes (1)
#define airfoildfe_bufferIDBits (3)
#define airfoildfe_qpadtPortPCIeWidthInBytes (160)
#define airfoildfe_mainSchedulePaddingBits (7)
#define airfoildfe_flushBufferIDBits (1)
#define airfoildfe_bufferAddressBits (9)
#define airfoildfe_flushSchedulePaddingBits (6)
#define airfoildfe_nPipes (2)
#define airfoildfe_totalResReadOnlyBits (528)
#define airfoildfe_floatBits (64)
#define airfoildfe_flushBufferAddressBits (7)


/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] ticks_AirfoilDFEAdtKernel The number of ticks for which kernel "AirfoilDFEAdtKernel" will run.
 * \param [in] ticks_AirfoilDFEResKernel The number of ticks for which kernel "AirfoilDFEResKernel" will run.
 * \param [in] ticks_AirfoilDFEUpdateKernel The number of ticks for which kernel "AirfoilDFEUpdateKernel" will run.
 * \param [in] inscalar_AirfoilDFEAdtKernel_cfl Input scalar parameter "AirfoilDFEAdtKernel.cfl".
 * \param [in] inscalar_AirfoilDFEAdtKernel_gam Input scalar parameter "AirfoilDFEAdtKernel.gam".
 * \param [in] inscalar_AirfoilDFEAdtKernel_gm1 Input scalar parameter "AirfoilDFEAdtKernel.gm1".
 * \param [in] inscalar_AirfoilDFEAdtKernel_numTicks Input scalar parameter "AirfoilDFEAdtKernel.numTicks".
 * \param [in] inscalar_AirfoilDFEResKernel_eps Input scalar parameter "AirfoilDFEResKernel.eps".
 * \param [in] inscalar_AirfoilDFEResKernel_gm1 Input scalar parameter "AirfoilDFEResKernel.gm1".
 * \param [in] inscalar_AirfoilDFEResKernel_nTicks Input scalar parameter "AirfoilDFEResKernel.nTicks".
 * \param [in] inscalar_AirfoilDFEUpdateKernel_doSaveQold Input scalar parameter "AirfoilDFEUpdateKernel.doSaveQold".
 * \param [in] inscalar_AirfoilDFEUpdateKernel_numCells Input scalar parameter "AirfoilDFEUpdateKernel.numCells".
 * \param [in] instream_cpu_qpadt_to_res Stream "cpu_qpadt_to_res".
 * \param [in] instream_size_cpu_qpadt_to_res The size of the stream instream_cpu_qpadt_to_res in bytes.
 * \param [in] instream_setupCPU Stream "setupCPU".
 * \param [in] instream_size_setupCPU The size of the stream instream_setupCPU in bytes.
 * \param [out] outstream_cpu_res_from_res Stream "cpu_res_from_res".
 * \param [in] outstream_size_cpu_res_from_res The size of the stream outstream_cpu_res_from_res in bytes.
 * \param [out] outstream_qCPUOut Stream "qCPUOut".
 * \param [in] outstream_size_qCPUOut The size of the stream outstream_qCPUOut in bytes.
 * \param [out] outstream_rmsOut Stream "rmsOut".
 * \param [in] outstream_size_rmsOut The size of the stream outstream_rmsOut in bytes.
 * \param [in] lmem_address_adtDxRead Linear LMem control for "adtDxRead" stream: base address, in bytes.
 * \param [in] lmem_arr_size_adtDxRead Linear LMem control for "adtDxRead" stream: array size, in bytes.
 * \param [in] lmem_address_adtQ Linear LMem control for "adtQ" stream: base address, in bytes.
 * \param [in] lmem_arr_size_adtQ Linear LMem control for "adtQ" stream: array size, in bytes.
 * \param [in] lmem_address_qRead Linear LMem control for "qRead" stream: base address, in bytes.
 * \param [in] lmem_arr_size_qRead Linear LMem control for "qRead" stream: array size, in bytes.
 * \param [in] lmem_address_resReadOnly Linear LMem control for "resReadOnly" stream: base address, in bytes.
 * \param [in] lmem_arr_size_resReadOnly Linear LMem control for "resReadOnly" stream: array size, in bytes.
 * \param [in] lmem_address_setupWrite Linear LMem control for "setupWrite" stream: base address, in bytes.
 * \param [in] lmem_arr_size_setupWrite Linear LMem control for "setupWrite" stream: array size, in bytes.
 * \param [in] lmem_address_updateQ Linear LMem control for "updateQ" stream: base address, in bytes.
 * \param [in] lmem_arr_size_updateQ Linear LMem control for "updateQ" stream: array size, in bytes.
 * \param [in] lmem_address_updateQold Linear LMem control for "updateQold" stream: base address, in bytes.
 * \param [in] lmem_arr_size_updateQold Linear LMem control for "updateQold" stream: array size, in bytes.
 * \param [in] lmem_address_updateSaveQold Linear LMem control for "updateSaveQold" stream: base address, in bytes.
 * \param [in] lmem_arr_size_updateSaveQold Linear LMem control for "updateSaveQold" stream: array size, in bytes.
 */
void airfoildfe(
	uint64_t ticks_AirfoilDFEAdtKernel,
	uint64_t ticks_AirfoilDFEResKernel,
	uint64_t ticks_AirfoilDFEUpdateKernel,
	double inscalar_AirfoilDFEAdtKernel_cfl,
	double inscalar_AirfoilDFEAdtKernel_gam,
	double inscalar_AirfoilDFEAdtKernel_gm1,
	uint64_t inscalar_AirfoilDFEAdtKernel_numTicks,
	double inscalar_AirfoilDFEResKernel_eps,
	double inscalar_AirfoilDFEResKernel_gm1,
	uint64_t inscalar_AirfoilDFEResKernel_nTicks,
	uint64_t inscalar_AirfoilDFEUpdateKernel_doSaveQold,
	uint64_t inscalar_AirfoilDFEUpdateKernel_numCells,
	const void *instream_cpu_qpadt_to_res,
	size_t instream_size_cpu_qpadt_to_res,
	const void *instream_setupCPU,
	size_t instream_size_setupCPU,
	void *outstream_cpu_res_from_res,
	size_t outstream_size_cpu_res_from_res,
	void *outstream_qCPUOut,
	size_t outstream_size_qCPUOut,
	void *outstream_rmsOut,
	size_t outstream_size_rmsOut,
	size_t lmem_address_adtDxRead,
	size_t lmem_arr_size_adtDxRead,
	size_t lmem_address_adtQ,
	size_t lmem_arr_size_adtQ,
	size_t lmem_address_qRead,
	size_t lmem_arr_size_qRead,
	size_t lmem_address_resReadOnly,
	size_t lmem_arr_size_resReadOnly,
	size_t lmem_address_setupWrite,
	size_t lmem_arr_size_setupWrite,
	size_t lmem_address_updateQ,
	size_t lmem_arr_size_updateQ,
	size_t lmem_address_updateQold,
	size_t lmem_arr_size_updateQold,
	size_t lmem_address_updateSaveQold,
	size_t lmem_arr_size_updateSaveQold);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] ticks_AirfoilDFEAdtKernel The number of ticks for which kernel "AirfoilDFEAdtKernel" will run.
 * \param [in] ticks_AirfoilDFEResKernel The number of ticks for which kernel "AirfoilDFEResKernel" will run.
 * \param [in] ticks_AirfoilDFEUpdateKernel The number of ticks for which kernel "AirfoilDFEUpdateKernel" will run.
 * \param [in] inscalar_AirfoilDFEAdtKernel_cfl Input scalar parameter "AirfoilDFEAdtKernel.cfl".
 * \param [in] inscalar_AirfoilDFEAdtKernel_gam Input scalar parameter "AirfoilDFEAdtKernel.gam".
 * \param [in] inscalar_AirfoilDFEAdtKernel_gm1 Input scalar parameter "AirfoilDFEAdtKernel.gm1".
 * \param [in] inscalar_AirfoilDFEAdtKernel_numTicks Input scalar parameter "AirfoilDFEAdtKernel.numTicks".
 * \param [in] inscalar_AirfoilDFEResKernel_eps Input scalar parameter "AirfoilDFEResKernel.eps".
 * \param [in] inscalar_AirfoilDFEResKernel_gm1 Input scalar parameter "AirfoilDFEResKernel.gm1".
 * \param [in] inscalar_AirfoilDFEResKernel_nTicks Input scalar parameter "AirfoilDFEResKernel.nTicks".
 * \param [in] inscalar_AirfoilDFEUpdateKernel_doSaveQold Input scalar parameter "AirfoilDFEUpdateKernel.doSaveQold".
 * \param [in] inscalar_AirfoilDFEUpdateKernel_numCells Input scalar parameter "AirfoilDFEUpdateKernel.numCells".
 * \param [in] instream_cpu_qpadt_to_res Stream "cpu_qpadt_to_res".
 * \param [in] instream_size_cpu_qpadt_to_res The size of the stream instream_cpu_qpadt_to_res in bytes.
 * \param [in] instream_setupCPU Stream "setupCPU".
 * \param [in] instream_size_setupCPU The size of the stream instream_setupCPU in bytes.
 * \param [out] outstream_cpu_res_from_res Stream "cpu_res_from_res".
 * \param [in] outstream_size_cpu_res_from_res The size of the stream outstream_cpu_res_from_res in bytes.
 * \param [out] outstream_qCPUOut Stream "qCPUOut".
 * \param [in] outstream_size_qCPUOut The size of the stream outstream_qCPUOut in bytes.
 * \param [out] outstream_rmsOut Stream "rmsOut".
 * \param [in] outstream_size_rmsOut The size of the stream outstream_rmsOut in bytes.
 * \param [in] lmem_address_adtDxRead Linear LMem control for "adtDxRead" stream: base address, in bytes.
 * \param [in] lmem_arr_size_adtDxRead Linear LMem control for "adtDxRead" stream: array size, in bytes.
 * \param [in] lmem_address_adtQ Linear LMem control for "adtQ" stream: base address, in bytes.
 * \param [in] lmem_arr_size_adtQ Linear LMem control for "adtQ" stream: array size, in bytes.
 * \param [in] lmem_address_qRead Linear LMem control for "qRead" stream: base address, in bytes.
 * \param [in] lmem_arr_size_qRead Linear LMem control for "qRead" stream: array size, in bytes.
 * \param [in] lmem_address_resReadOnly Linear LMem control for "resReadOnly" stream: base address, in bytes.
 * \param [in] lmem_arr_size_resReadOnly Linear LMem control for "resReadOnly" stream: array size, in bytes.
 * \param [in] lmem_address_setupWrite Linear LMem control for "setupWrite" stream: base address, in bytes.
 * \param [in] lmem_arr_size_setupWrite Linear LMem control for "setupWrite" stream: array size, in bytes.
 * \param [in] lmem_address_updateQ Linear LMem control for "updateQ" stream: base address, in bytes.
 * \param [in] lmem_arr_size_updateQ Linear LMem control for "updateQ" stream: array size, in bytes.
 * \param [in] lmem_address_updateQold Linear LMem control for "updateQold" stream: base address, in bytes.
 * \param [in] lmem_arr_size_updateQold Linear LMem control for "updateQold" stream: array size, in bytes.
 * \param [in] lmem_address_updateSaveQold Linear LMem control for "updateSaveQold" stream: base address, in bytes.
 * \param [in] lmem_arr_size_updateSaveQold Linear LMem control for "updateSaveQold" stream: array size, in bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *airfoildfe_nonblock(
	uint64_t ticks_AirfoilDFEAdtKernel,
	uint64_t ticks_AirfoilDFEResKernel,
	uint64_t ticks_AirfoilDFEUpdateKernel,
	double inscalar_AirfoilDFEAdtKernel_cfl,
	double inscalar_AirfoilDFEAdtKernel_gam,
	double inscalar_AirfoilDFEAdtKernel_gm1,
	uint64_t inscalar_AirfoilDFEAdtKernel_numTicks,
	double inscalar_AirfoilDFEResKernel_eps,
	double inscalar_AirfoilDFEResKernel_gm1,
	uint64_t inscalar_AirfoilDFEResKernel_nTicks,
	uint64_t inscalar_AirfoilDFEUpdateKernel_doSaveQold,
	uint64_t inscalar_AirfoilDFEUpdateKernel_numCells,
	const void *instream_cpu_qpadt_to_res,
	size_t instream_size_cpu_qpadt_to_res,
	const void *instream_setupCPU,
	size_t instream_size_setupCPU,
	void *outstream_cpu_res_from_res,
	size_t outstream_size_cpu_res_from_res,
	void *outstream_qCPUOut,
	size_t outstream_size_qCPUOut,
	void *outstream_rmsOut,
	size_t outstream_size_rmsOut,
	size_t lmem_address_adtDxRead,
	size_t lmem_arr_size_adtDxRead,
	size_t lmem_address_adtQ,
	size_t lmem_arr_size_adtQ,
	size_t lmem_address_qRead,
	size_t lmem_arr_size_qRead,
	size_t lmem_address_resReadOnly,
	size_t lmem_arr_size_resReadOnly,
	size_t lmem_address_setupWrite,
	size_t lmem_arr_size_setupWrite,
	size_t lmem_address_updateQ,
	size_t lmem_arr_size_updateQ,
	size_t lmem_address_updateQold,
	size_t lmem_arr_size_updateQold,
	size_t lmem_address_updateSaveQold,
	size_t lmem_arr_size_updateSaveQold);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	uint64_t ticks_AirfoilDFEAdtKernel; /**<  [in] The number of ticks for which kernel "AirfoilDFEAdtKernel" will run. */
	uint64_t ticks_AirfoilDFEResKernel; /**<  [in] The number of ticks for which kernel "AirfoilDFEResKernel" will run. */
	uint64_t ticks_AirfoilDFEUpdateKernel; /**<  [in] The number of ticks for which kernel "AirfoilDFEUpdateKernel" will run. */
	double inscalar_AirfoilDFEAdtKernel_cfl; /**<  [in] Input scalar parameter "AirfoilDFEAdtKernel.cfl". */
	double inscalar_AirfoilDFEAdtKernel_gam; /**<  [in] Input scalar parameter "AirfoilDFEAdtKernel.gam". */
	double inscalar_AirfoilDFEAdtKernel_gm1; /**<  [in] Input scalar parameter "AirfoilDFEAdtKernel.gm1". */
	uint64_t inscalar_AirfoilDFEAdtKernel_numTicks; /**<  [in] Input scalar parameter "AirfoilDFEAdtKernel.numTicks". */
	double inscalar_AirfoilDFEResKernel_eps; /**<  [in] Input scalar parameter "AirfoilDFEResKernel.eps". */
	double inscalar_AirfoilDFEResKernel_gm1; /**<  [in] Input scalar parameter "AirfoilDFEResKernel.gm1". */
	uint64_t inscalar_AirfoilDFEResKernel_nTicks; /**<  [in] Input scalar parameter "AirfoilDFEResKernel.nTicks". */
	uint64_t inscalar_AirfoilDFEUpdateKernel_doSaveQold; /**<  [in] Input scalar parameter "AirfoilDFEUpdateKernel.doSaveQold". */
	uint64_t inscalar_AirfoilDFEUpdateKernel_numCells; /**<  [in] Input scalar parameter "AirfoilDFEUpdateKernel.numCells". */
	const void *instream_cpu_qpadt_to_res; /**<  [in] Stream "cpu_qpadt_to_res". */
	size_t instream_size_cpu_qpadt_to_res; /**<  [in] The size of the stream instream_cpu_qpadt_to_res in bytes. */
	const void *instream_setupCPU; /**<  [in] Stream "setupCPU". */
	size_t instream_size_setupCPU; /**<  [in] The size of the stream instream_setupCPU in bytes. */
	void *outstream_cpu_res_from_res; /**<  [out] Stream "cpu_res_from_res". */
	size_t outstream_size_cpu_res_from_res; /**<  [in] The size of the stream outstream_cpu_res_from_res in bytes. */
	void *outstream_qCPUOut; /**<  [out] Stream "qCPUOut". */
	size_t outstream_size_qCPUOut; /**<  [in] The size of the stream outstream_qCPUOut in bytes. */
	void *outstream_rmsOut; /**<  [out] Stream "rmsOut". */
	size_t outstream_size_rmsOut; /**<  [in] The size of the stream outstream_rmsOut in bytes. */
	size_t lmem_address_adtDxRead; /**<  [in] Linear LMem control for "adtDxRead" stream: base address, in bytes. */
	size_t lmem_arr_size_adtDxRead; /**<  [in] Linear LMem control for "adtDxRead" stream: array size, in bytes. */
	size_t lmem_address_adtQ; /**<  [in] Linear LMem control for "adtQ" stream: base address, in bytes. */
	size_t lmem_arr_size_adtQ; /**<  [in] Linear LMem control for "adtQ" stream: array size, in bytes. */
	size_t lmem_address_qRead; /**<  [in] Linear LMem control for "qRead" stream: base address, in bytes. */
	size_t lmem_arr_size_qRead; /**<  [in] Linear LMem control for "qRead" stream: array size, in bytes. */
	size_t lmem_address_resReadOnly; /**<  [in] Linear LMem control for "resReadOnly" stream: base address, in bytes. */
	size_t lmem_arr_size_resReadOnly; /**<  [in] Linear LMem control for "resReadOnly" stream: array size, in bytes. */
	size_t lmem_address_setupWrite; /**<  [in] Linear LMem control for "setupWrite" stream: base address, in bytes. */
	size_t lmem_arr_size_setupWrite; /**<  [in] Linear LMem control for "setupWrite" stream: array size, in bytes. */
	size_t lmem_address_updateQ; /**<  [in] Linear LMem control for "updateQ" stream: base address, in bytes. */
	size_t lmem_arr_size_updateQ; /**<  [in] Linear LMem control for "updateQ" stream: array size, in bytes. */
	size_t lmem_address_updateQold; /**<  [in] Linear LMem control for "updateQold" stream: base address, in bytes. */
	size_t lmem_arr_size_updateQold; /**<  [in] Linear LMem control for "updateQold" stream: array size, in bytes. */
	size_t lmem_address_updateSaveQold; /**<  [in] Linear LMem control for "updateSaveQold" stream: base address, in bytes. */
	size_t lmem_arr_size_updateSaveQold; /**<  [in] Linear LMem control for "updateSaveQold" stream: array size, in bytes. */
} airfoildfe_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void airfoildfe_run(
	max_engine_t *engine,
	airfoildfe_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'default'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *airfoildfe_run_nonblock(
	max_engine_t *engine,
	airfoildfe_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void airfoildfe_run_group(max_group_t *group, airfoildfe_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *airfoildfe_run_group_nonblock(max_group_t *group, airfoildfe_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void airfoildfe_run_array(max_engarray_t *engarray, airfoildfe_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *airfoildfe_run_array_nonblock(max_engarray_t *engarray, airfoildfe_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* airfoildfe_convert(max_file_t *maxfile, airfoildfe_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* airfoildfe_init(void);

/* Error handling functions */
int airfoildfe_has_errors(void);
const char* airfoildfe_get_errors(void);
void airfoildfe_clear_errors(void);
/* Free statically allocated maxfile data */
void airfoildfe_free(void);
/* These are dummy functions for hardware builds. */
int airfoildfe_simulator_start(void);
int airfoildfe_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_airfoildfe_H */

