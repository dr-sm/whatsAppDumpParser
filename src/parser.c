#include <locale.h>
#include "extend.h"
#include "utilities.h"
#include "arrayList.h"
#include "bool.h"

int main(int argc, char** argv) {
	if ( validateArgs (argc) ) {
		help_screen();
		return 0;
	}

	setlocale(LC_ALL, "");

	char* garbage_list[100] = {
					"You deleted this message",
					"Media omitted",
                                        "Missed voice call",
                                        "Пропущенный голосовой вызов",
                                        "Без медиафайлов",
                                        "Messages and calls are end-to-end",
                                        "Сообщения и звонки защищены сквозным шифрованием",
					"This message was deleted",
					"file attached",
					"Данное сообщение удалено"
			        };

	char str[RBMS] = "";
	struct ArrayList_str* finded_names = NULL;
	if ( !(finded_names = (struct ArrayList_str*) malloc (sizeof(struct ArrayList_str))) )
		return 0;
	arrayList_str_init(&finded_names);

	char* name = NULL;

	struct Path *path_struct = NULL;
	if ( !(path_struct = (struct Path*) malloc (sizeof(struct Path))) ) return 0;
	path_init(&path_struct);

	strcpy(path_struct->dir_path, "./out.d/");
	strcpy(path_struct->prefix, argv[1]);
	strcpy(path_struct->format, ".txt");

	_Bool isReadable = FALSE;
	while ( isReadable = readFromStream(str) == TRUE ) {
		if ( filter_wrong_format(str,
			"[0-2]{1}[0-9]{1}[/.]{1}[0-2]{1}[0-9]{1}[/.]{1}([0-9]{4}|[0-9]{2}),\\s[0-2]{1}[0-9]{1}:[0-9]{1}[0-9]{1}+\\s-\\s[a-zA-Zа-яА-Я0-9:,.; -+]+:") == FALSE)
			continue;
		if ( filter_owner_msg(str, &finded_names, &name) == FALSE) continue;
		if ( filter_delete_meta(str, DATETIME) == FALSE ) continue;
		if ( filter_delete_meta(str, NAME) == FALSE ) continue;
		if ( filter_delete_garbage(str, garbage_list, 10) == FALSE ) continue;

		path_struct->file_name = name;
		writeToStream(str, path_struct);
	}

	path_free(&path_struct);
	arrayList_str_free(&finded_names);

        return 0;
}
