#ifndef _JF_SHARED
#define _JF_SHARED

#include <pthread.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>

// for hardcoded strings
#define JF_STATIC_STRLEN(str) (sizeof(str) - 1)

#define JF_VERSION "prealpha"

#define JF_THREAD_BUFFER_DATA_SIZE (CURL_MAX_WRITE_SIZE +1)

#define JF_ID_LENGTH 32

#define JF_CONFIG_DEVICEID_MAX_LEN 32


////////// GENERIC JELLYFIN ITEM REPRESENTATION //////////
// Information about persistency is used to make part of the menu interface
// tree not get deallocated when navigating upwards
typedef char jf_item_type;

// Atoms
#define JF_ITEM_TYPE_NONE			0
#define JF_ITEM_TYPE_AUDIO			1
#define JF_ITEM_TYPE_EPISODE		2
#define JF_ITEM_TYPE_MOVIE			3
#define JF_ITEM_TYPE_AUDIOBOOK		4

// Special menu commands
#define JF_ITEM_TYPE_COMMAND_QUIT	10

// Folders
#define JF_ITEM_TYPE_COLLECTION		20
#define JF_ITEM_TYPE_USER_VIEW		21
#define JF_ITEM_TYPE_FOLDER			22
#define JF_ITEM_TYPE_PLAYLIST		23
#define JF_ITEM_TYPE_ARTIST			24
#define JF_ITEM_TYPE_ALBUM			25
#define JF_ITEM_TYPE_SEASON			26
#define JF_ITEM_TYPE_SERIES			27

// Persistent folders
#define JF_ITEM_TYPE_MENU_ROOT		-1
#define JF_ITEM_TYPE_MENU_FAVORITES	-2
#define JF_ITEM_TYPE_MENU_ON_DECK	-3
#define JF_ITEM_TYPE_MENU_LATEST	-4
#define JF_ITEM_TYPE_MENU_LIBRARIES	-5

// Category macros. They're all expressions
// UPDATE THESE if you add item_type's or change the item_type representation!
#define JF_MENU_ITEM_TYPE_IS_PERSISTENT(t)			((t) < 0)
#define JF_MENU_ITEM_TYPE_IS_FOLDER(t)				((t) < 0 || (t) >= 20)
#define JF_MENU_ITEM_TYPE_HAS_DYNAMIC_CHILDREN(t)	((t) < -1 || (t) >= 20)
//////////////////////////////////////////////////////////


typedef struct jf_menu_item {
	jf_item_type type;
	char id[JF_ID_LENGTH +1];
	struct jf_menu_item *children; // NULL-terminated
} jf_menu_item;


////////// THREAD_BUFFER //////////
typedef unsigned char jf_thread_buffer_state;

#define JF_THREAD_BUFFER_STATE_CLEAR			0
#define JF_THREAD_BUFFER_STATE_AWAITING_DATA	1
#define JF_THREAD_BUFFER_STATE_PENDING_DATA		2
#define JF_THREAD_BUFFER_STATE_PARSER_ERROR		3
#define JF_THREAD_BUFFER_STATE_PARSER_DEAD		4


typedef struct jf_thread_buffer {
	char data[JF_THREAD_BUFFER_DATA_SIZE];
	size_t used;
	bool promiscuous_context;
	jf_thread_buffer_state state;
	unsigned char *parsed_ids;
	size_t parsed_ids_size;
	size_t item_count;
	pthread_mutex_t mut;
	pthread_cond_t cv_no_data;
	pthread_cond_t cv_has_data;
} jf_thread_buffer;


bool jf_thread_buffer_init(jf_thread_buffer *tb);
///////////////////////////////////




// UNUSED FOR NOW
// typedef struct jf_synced_queue {
// 	const void **slots;
// 	size_t slot_count;
// 	size_t current;
// 	size_t next;
// 	pthread_mutex_t mut;
// 	pthread_cond_t cv_is_empty;
// 	pthread_cond_t cv_is_full;
// } jf_synced_queue;


// returns a NULL-terminated, malloc'd string result of the concatenation of its (expected char *) arguments past the first
// the first argument is the number of following arguments
char *jf_concat(const size_t n, ...);



// UNUSED FOR NOW
// jf_synced_queue *jf_synced_queue_new(const size_t slot_count);
// void jf_synced_queue_free(jf_synced_queue *q); // NB will NOT deallocate the contents of the queue! make sure it's empty beforehand to avoid leaks
// void jf_synced_queue_enqueue(jf_synced_queue *q, const void *payload);
// void *jf_synced_queue_dequeue(jf_synced_queue *q);
// size_t jf_synced_queue_is_empty(const jf_synced_queue *q);

#endif
