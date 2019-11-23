#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "iw.h"
#include "error.h"
#include "ap_list.h"


void init_ap_list(struct ap_list *apl) {
	apl->head = NULL;
	apl->tail = NULL;
}

void free_ap_list(struct ap_list *apl) {
	struct access_point *tmp;

	while (apl->head != NULL) {
		tmp = apl->head;
		apl->head = apl->head->next;
		free(tmp);
	}

	apl->head = apl->tail = NULL;
}

void link_ap(struct ap_list *apl, struct access_point *ap) {
	if (apl->head == NULL)
		apl->head = apl->tail = ap;
	else {
		ap->prev = apl->tail;
		apl->tail->next = ap;
		apl->tail = ap;
	}
}

void unlink_ap(struct ap_list *apl, struct access_point *ap) {
	if (ap->prev)
		ap->prev->next = ap->next;
	else
		apl->head = ap->next;
	if (ap->next)
		ap->next->prev = ap->prev;
	else
		apl->tail = ap->prev;
}

int add_or_update_ap(struct ap_list *apl, struct ap_info *api) {
	struct access_point *ap;

	ap = apl->head;
	while (ap != NULL) {
		if (memcmp(ap->info.bssid, api->bssid, IFHWADDRLEN) == 0)
			break;
		ap = ap->next;
	}

	if (ap == NULL) {
		ap = malloc(sizeof(*ap));
		if (ap == NULL)
			return_error("malloc");

		memset(ap, 0, sizeof(*ap));
		memcpy(&ap->info, api, sizeof(ap->info));
		ap->last_beacon_tm = time(NULL);
		link_ap(apl, ap);
	} else
		ap->last_beacon_tm = time(NULL);

	return 0;
}
