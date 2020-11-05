/*
 * ntrdma_cm.h
 *
 *  Created on: Jan 1, 2020
 *      Author: leonidr
 */

#ifndef DRIVERS_INFINIBAND_HW_NTRDMA_NTRDMA_CM_H_
#define DRIVERS_INFINIBAND_HW_NTRDMA_NTRDMA_CM_H_

#include <rdma/iw_cm.h>
#include "ntrdma_cmd.h"
#include "ntrdma_dev.h"

enum ntrdma_cm_state {
	NTRDMA_CM_STATE_IDLE = 0,
	NTRDMA_CM_STATE_ESTABLISHED,
	NTRDMA_CM_STATE_CONNECTING,
	NTRDMA_CM_STATE_KILLING,
};

#define iwcm_2_ntrdma_iwcm(__iwcm) \
	__iwcm->iwcm

#define ntrdma_iw_cm_from_ntrdma_dev(__dev) \
	iwcm_2_ntrdma_iwcm(__dev)


struct ntrdma_iw_cm {
	struct list_head ntrdma_iw_cm_list;
	struct kmem_cache *cmid_node_slab;
	rwlock_t slock;
};

struct ntrdma_iw_cm_id_node {
	struct list_head head;
	struct iw_cm_id *cm_id;
	int qpn;
};

int ntrdma_cmd_recv_cm(struct ntrdma_dev *dev,
		const union ntrdma_cmd *cmd,
		union ntrdma_rsp *rsp);

void ntrdma_cm_add_ref(struct ib_qp *ibqp);
void ntrdma_cm_rem_ref(struct ib_qp *ibqp);
struct ib_qp *ntrdma_get_qp(struct ib_device *ibdev, int qpn);
int ntrdma_connect(struct iw_cm_id *cm_id, struct iw_cm_conn_param *conn_param);
int ntrdma_accept(struct iw_cm_id *cm_id, struct iw_cm_conn_param *conn_param);
int ntrdma_reject(struct iw_cm_id *cm_id, const void *pdata, u8 pdata_len);
int ntrdma_create_listen(struct iw_cm_id *cm_id, int backlog);
int ntrdma_destroy_listen(struct iw_cm_id *cm_id);

struct ntrdma_iw_cm *ntrdma_cm_init(struct ntrdma_dev *dev);

void ntrdma_cm_deinit(struct ntrdma_dev *iwcm);

void ntrdma_cm_qp_shutdown(struct ntrdma_qp *qp);
void ntrdma_cm_kill(struct ntrdma_qp *qp);
int _ntrdma_modify_qp_local(struct ib_qp *ibqp,
		struct ib_qp_attr *ibqp_attr,
		int ibqp_mask,
		const char *func);

#define ntrdma_modify_qp_local(_ibqp, _ibqp_attr, _ibqp_mask) \
		_ntrdma_modify_qp_local(_ibqp, _ibqp_attr, _ibqp_mask, __func__)



#endif /* DRIVERS_INFINIBAND_HW_NTRDMA_NTRDMA_CM_H_ */
