#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// HTML tags
#define HTML_START              "<html>"
#define HTML_END                "</html>"

#define H1_START                "<h1>"
#define H1_END                  "</h1>"

#define H2_START                "<h2>"
#define H2_END                  "</h2>"

#define H3_START                "<h3>"
#define H3_END                  "</h3>"

#define H4_START                "<h4>"
#define H4_END                  "</h4>"

#define LIST_START              "<ul>"
#define LIST_ITEM_START         "<li>"
#define LIST_ITEM_END           "</li>"
#define LIST_END                "</ul>"

#define PARA_START              "<p>"
#define PARA_END                "</p>"

#define BOLD_START              "<b>"
#define BOLD_END                "</b>"

#define ITALIC_START            "<em>"
#define ITALIC_END              "</em>"

#define BOLDITALIC_START        "<b><em>"
#define BOLDITALIC_END          "</b></em>"

#define HYPERLINK_START         "<a href=\""
#define HYPERLINK_MIDDLE        "\">"
#define HYPERLINK_END           "</a>"

// MD tags
#define MD_HEADING1             "#"
#define MD_HEADING2             "##"
#define MD_HEADING3             "###"
#define MD_HEADING4             "####"

#define MD_LIST                 "* "

#define MD_ITALIC               "*"
#define MD_BOLD                 "**"
#define MD_BOLDITALIC           "***"

#define MD_HYPERLINK_START      "["
#define MD_HYPERLINK_MIDDLE     "]("
#define MD_HYPERLINK_END        ")"


typedef struct FileInfo {
	size_t	number_of_lines;
	char**	line_text;
} FileInfo;

FileInfo get_file_lines(char* file_name) {
	FILE* fp;
	char* buffer = NULL;
	size_t bufsize = 1000;
	ssize_t read;

	int lines_total = 0;
	int len_total = 0;

	// We go through the file once to get the total number of lines
	fp = fopen(file_name, "r");
	if (fp == NULL) exit(EXIT_FAILURE);
	while ((read = getline(&buffer, &bufsize, fp)) != -1) {
		lines_total++;
		len_total += strlen(buffer);
	}
	fclose(fp);
	free(buffer);

	// An array of pointers is created with the size of the total number of lines
	char* file_text[lines_total];

	buffer = (char*)malloc(bufsize * sizeof(char));

	// We go through the file AGAIN to actually get the content this time
	fp = fopen(file_name, "r");
	if (fp == NULL) exit(EXIT_FAILURE);
	int i = 0;
	while ((read = getline(&buffer, &bufsize, fp)) != -1) {
		file_text[i] = (char*)malloc(bufsize * sizeof(char));
		strcpy(file_text[i], buffer);
		i++;
	}

	fclose(fp);
	free(buffer);

	FileInfo file = {
		.number_of_lines = lines_total,
		.line_text = (char**)malloc(bufsize * sizeof(char)),
	};

	for (int i = 0; i < file.number_of_lines; i++) {
		file.line_text[i] = file_text[i];
	}

	return file;
}

// There's definitely a better way to do this
FileInfo get_stdin_lines(FILE* input) {
	// So we redirect stdout to a temp file, then we fread through stdin
	// and print output buffer TO the temp file
	// the temp file then gets read by the regular get_file_lines func
	// stdout then gets redirected back to the terminal and the temp file removed
	int o_stdout = dup(1);

	FILE* inp;
	FileInfo file;

	FILE* fp = input;
	char* buffer = NULL;
	size_t bufsize = 1000;
	ssize_t read;

	buffer = (char*)malloc(bufsize * sizeof(char));

	inp = freopen("/var/tmp/cax.input", "w", stdout);
	if (inp == NULL) perror("fropen");

	while ((read = getline(&buffer, &bufsize, fp)) != -1) {
		printf("%s", buffer);
	}

	fclose(fp);

	FILE* fp2 = fdopen(o_stdout, "w");

	fclose(stdout);
	stdout = fp2; 

	file = get_file_lines("/var/tmp/cax.input");

	remove("/var/tmp/cax.input");

	return file;
}

// Very straightforward but the memory allocation amounts need to be looked at
// could probably use better string funcs too
FileInfo build_headers(FileInfo file) {
	char* buffer;
	char* new_header;

	for (int i = 0; i < file.number_of_lines; i++) {
		if (strncmp(file.line_text[i], MD_HEADING4, 4) == 0) {
		// H4
			buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
			new_header = (char*)malloc(1000 * sizeof(char));

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 4];
			}

			strcat(new_header, H4_START);
			strcat(new_header, buffer);
			strcat(new_header, H4_END);
			strcpy(file.line_text[i], new_header);

		} else if (strncmp(file.line_text[i], MD_HEADING3, 3) == 0) {
		// H3
			buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
			new_header = (char*)malloc(1000 * sizeof(char));

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 3];
			}

			strcat(new_header, H3_START);
			strcat(new_header, buffer);
			strcat(new_header, H3_END);
			strcpy(file.line_text[i], new_header);
		} else if (strncmp(file.line_text[i], MD_HEADING2, 2) == 0) {
		// H2
			buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
			new_header = (char*)malloc(1000 * sizeof(char));

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 2];
			}

			strcat(new_header, H2_START);
			strcat(new_header, buffer);
			strcat(new_header, H2_END);
			strcpy(file.line_text[i], new_header);
		} else if (strncmp(file.line_text[i], MD_HEADING1, 1) == 0) {
		// H1
			buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
			new_header = (char*)malloc(1000 * sizeof(char));

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 1];
			}

			strcat(new_header, H1_START);
			strcat(new_header, buffer);
			strcat(new_header, H1_END);
			strcpy(file.line_text[i], new_header);
		}
	}
	return file;
}

FileInfo build_lists(FileInfo file) {
	char* buffer;
	char* list_item;
	int count = 0;
	int emer_count = 100;
	int len = 0;

	for (int i = 0; i < file.number_of_lines; i++) {
		len = strlen(file.line_text[i]);
		if (file.line_text[i][0] == '*' && file.line_text[i][1] == ' ') {
			count++;
		}
	}

	// Convert the individual list items
	while (count > 1) {
		emer_count -= 1;
		if (emer_count == 0) {
			printf("Error: list loop failure\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < file.number_of_lines; i++) {
			if (strncmp(file.line_text[i], MD_LIST, 2) == 0) {
				buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
				list_item = (char*)malloc(1000 * sizeof(char));

				for (int j = 0; j < strlen(file.line_text[i]); j++) {
					buffer[j]  = file.line_text[i][j + 2];
				}

				strcat(list_item, LIST_ITEM_START);
				strcat(list_item, buffer);
				strcat(list_item, LIST_ITEM_END);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;

				count--;
			}
		}
	}

	// Add the unordered list starting tag before the list items
	for (int i = 0; i < file.number_of_lines; i++) {
		if (!strstr(file.line_text[i], LIST_ITEM_END)) {
			if (i == file.number_of_lines - 1) break;
			if (strstr(file.line_text[i + 1], LIST_ITEM_START)) {
				buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
				list_item = (char*)malloc(1000 * sizeof(char));

				strcpy(buffer, file.line_text[i]);

				strcat(list_item, LIST_START);
				strcat(list_item, buffer);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;
			}
		}
	}

	// Add the unordered list ending tag after the list items
	for (int i = 0; i < file.number_of_lines; i++) {
		if (strstr(file.line_text[i], LIST_ITEM_START)) {
			if (i == file.number_of_lines - 2) {
				buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
				list_item = (char*)malloc(1000 * sizeof(char));

				strcpy(buffer, file.line_text[i]);

				strcat(list_item, buffer);
				strcat(list_item, LIST_END);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;

				break;
			}

			int index = strlen(file.line_text[i + 1]);
			char* last_five = &file.line_text[i + 1][index - 5];
			if (strncmp(last_five, LIST_ITEM_END, 5)) {
				buffer = (char*)malloc(strlen(file.line_text[i]) * sizeof(char));
				list_item = (char*)malloc(1000 * sizeof(char));

				strcpy(buffer, file.line_text[i]);

				strcat(list_item, buffer);
				strcat(list_item, LIST_END);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;
			}
		}
	}

	return file;
}

FileInfo build_paras(FileInfo file) {
	char* buffer;
	char* new_line;

	for (int i = 0; i < file.number_of_lines; i++) {
		int line_length = strlen(file.line_text[i]);
		if (strstr(file.line_text[i], "  ")) {
			new_line = (char*)malloc(2000 * sizeof(char));
			strcat(new_line, PARA_START);
			strcat(new_line, file.line_text[i]);
			strcat(new_line, PARA_END);
			strcpy(file.line_text[i], new_line);
		}
	}

	return file;
}

// This works but it's very ugly and I hate it
FileInfo build_emphasis(FileInfo file) {
	char* search_buffer = (char*)malloc(2000 * sizeof(char));
	char* buffer = (char*)malloc(2000 * sizeof(char));
	char* before_text = (char*)malloc(2000 * sizeof(char));
	char* after_text = (char*)malloc(2000 * sizeof(char));
	char* new_line = (char*)malloc(2000 * sizeof(char));

	int count = 0;
	int emer_count = 0;
	int len = 0;
	int line_length = 0;
	int index1 = 0;
	int index2 = 0;

	for (int i = 0; i < file.number_of_lines; i++) {
		len = strlen(file.line_text[i]);
		for (int j = 0; j < len; j++) {
			if (file.line_text[i][j] == '*' && file.line_text[i][j + 1] != ' ') {
				count++;
			}
		}
	}

	while (count > 1) {
		emer_count -= 1;
		if (emer_count == 0) {
			printf("Error: emphasis loop failure\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < file.number_of_lines; i++) {
			if ((search_buffer = strstr(file.line_text[i], MD_BOLDITALIC)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				if (!strncmp(&file.line_text[i][index1 + 2], "*", 1)) {
					search_buffer = (char*)malloc(2000 * sizeof(char));
					buffer = (char*)malloc(2000 * sizeof(char));
					before_text = (char*)malloc(2000 * sizeof(char));
					after_text = (char*)malloc(2000 * sizeof(char));
					new_line = (char*)malloc(2000 * sizeof(char));

					if ((search_buffer = strstr(file.line_text[i] + index1 + 2, MD_BOLDITALIC)) == NULL) continue;
					index2 = search_buffer - file.line_text[i];
					strncpy(buffer, file.line_text[i] + index1 + 3, index2 - index1 - 2);

					strncpy(before_text, file.line_text[i], index1);
					strncpy(after_text, file.line_text[i] + index2 + 3, line_length - index2);

					strcat(new_line, before_text);
					strcat(new_line, BOLDITALIC_START);

					strcat(new_line, buffer);
					strcat(new_line, BOLDITALIC_END);

					strcat(new_line, after_text);
					strcpy(file.line_text[i], new_line);

					buffer = NULL;
					before_text = NULL;
					after_text = NULL;
					new_line = NULL;

					count -= 6;
				}
			}

			if ((search_buffer = strstr(file.line_text[i], MD_BOLD)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				 if (!strncmp(&file.line_text[i][index1 + 1], "*", 1)) {
					search_buffer = (char*)malloc(2000 * sizeof(char));
					buffer = (char*)malloc(2000 * sizeof(char));
					before_text = (char*)malloc(2000 * sizeof(char));
					after_text = (char*)malloc(2000 * sizeof(char));
					new_line = (char*)malloc(2000 * sizeof(char));

					if ((search_buffer = strstr(file.line_text[i] + index1 + 2, MD_BOLD)) == NULL) continue;
					index2 = search_buffer - file.line_text[i];
					strncpy(buffer, file.line_text[i] + index1 + 2, index2 - index1 - 2);

					strncpy(before_text, file.line_text[i], index1);
					strncpy(after_text, file.line_text[i] + index2 + 2, line_length - index2);

					strcat(new_line, before_text);
					strcat(new_line, BOLD_START);

					strcat(new_line, buffer);
					strcat(new_line, BOLD_END);

					strcat(new_line, after_text);
					strcpy(file.line_text[i], new_line);

					buffer = NULL;
					before_text = NULL;
					after_text = NULL;
					new_line = NULL;

					count -= 4;
				 }
			}

			if ((search_buffer = strstr(file.line_text[i], MD_ITALIC)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				if (!strncmp(&file.line_text[i][index1], "*", 1)) {
					search_buffer = (char*)malloc(2000 * sizeof(char));
					buffer = (char*)malloc(2000 * sizeof(char));
					before_text = (char*)malloc(2000 * sizeof(char));
					after_text = (char*)malloc(2000 * sizeof(char));
					new_line = (char*)malloc(2000 * sizeof(char));

					if ((search_buffer = strstr(file.line_text[i] + index1 + 1, MD_ITALIC)) == NULL) continue;
					index2 = search_buffer - file.line_text[i];
					strncpy(buffer, file.line_text[i] + index1 + 1, index2 - index1 - 1);

					strncpy(before_text, file.line_text[i], index1);
					strncpy(after_text, file.line_text[i] + index2 + 1, line_length - index2);

					strcat(new_line, before_text);
					strcat(new_line, ITALIC_START);

					strcat(new_line, buffer);
					strcat(new_line, ITALIC_END);

					strcat(new_line, after_text);
					strcpy(file.line_text[i], new_line);

					buffer = NULL;
					before_text = NULL;
					after_text = NULL;
					new_line = NULL;

					count -= 2;
				}
			}
		}
	}

	return file;
}

FileInfo build_hyperlinks(FileInfo file) {
	char* search_buffer = (char*)malloc(2000 * sizeof(char));
	char* urltext_buffer = (char*)malloc(2000 * sizeof(char));
	char* url_buffer = (char*)malloc(2000 * sizeof(char));
	char* before_text = (char*)malloc(2000 * sizeof(char));
	char* after_text = (char*)malloc(2000 * sizeof(char));
	char* new_line = (char*)malloc(2000 * sizeof(char));

	int count = 0;
	int emer_count = 100;
	int len = 0;
	int line_length = 0;
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;

	for (int i = 0; i < file.number_of_lines; i++) {
		len = strlen(file.line_text[i]);
		for (int j = 0; j < len; j++) {
			if (file.line_text[i][j] == '[') count++;
		}
	}

	while (count > 1) {
		emer_count -= 1;
		if (emer_count == 0) { 
			printf("Error: hyperlink loop failure\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < file.number_of_lines; i++) {
			if ((search_buffer = strstr(file.line_text[i], MD_HYPERLINK_START)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				search_buffer = (char*)malloc(2000 * sizeof(char));
				urltext_buffer = (char*)malloc(2000 * sizeof(char));
				url_buffer = (char*)malloc(2000 * sizeof(char));
				before_text = (char*)malloc(2000 * sizeof(char));
				after_text = (char*)malloc(2000 * sizeof(char));
				new_line = (char*)malloc(2000 * sizeof(char));

				if ((search_buffer = strstr(file.line_text[i] + index1 + 1, MD_HYPERLINK_MIDDLE)) == NULL) continue;
				index2 = search_buffer - file.line_text[i];
				strncpy(urltext_buffer, file.line_text[i] + index1 + 1, index2 - index1 - 1);
				if ((search_buffer = strstr(file.line_text[i] + index2 + 1, MD_HYPERLINK_END)) == NULL) continue;
				index3 = search_buffer - file.line_text[i];
				strncpy(url_buffer, file.line_text[i] + index2 + 2, index3 - index2 - 2);

				strncpy(before_text, file.line_text[i], index1);
				strncpy(after_text, file.line_text[i] + index3 + 1, line_length - index2);

				strcat(new_line, before_text);
				strcat(new_line, HYPERLINK_START);

				strcat(new_line, url_buffer);
				strcat(new_line, HYPERLINK_MIDDLE);
				strcat(new_line, urltext_buffer);
				strcat(new_line, HYPERLINK_END);

				strcat(new_line, after_text);
				strcpy(file.line_text[i], new_line);

				urltext_buffer = NULL;
				url_buffer = NULL;
				before_text = NULL;
				after_text = NULL;
				new_line = NULL;

				count--;
			}
		}
	}


	return file;
}

// This isn't necessarily the best way to do this either but it works
FileInfo build_html(FileInfo file) {
	int new_line_total = file.number_of_lines + 2;
	char* html_file_lines[new_line_total];

	html_file_lines[0] = (char*)malloc(1000 * sizeof(char));
	html_file_lines[0] = HTML_START;
	html_file_lines[new_line_total - 1] = (char*)malloc(1000 * sizeof(char));
	html_file_lines[new_line_total - 1] = HTML_END;

	for (int i = 1; i < new_line_total; i++) {
		html_file_lines[i] = (char*)malloc(1000 * sizeof(char));
		html_file_lines[i] = file.line_text[i - 1];
		if (i == file.number_of_lines) break;
	}

	FileInfo new_file = {
		.number_of_lines = new_line_total,
		.line_text = (char**)malloc(1000 * sizeof(char)),
	};

	for (int i = 0; i < new_file.number_of_lines; i++) {
		new_file.line_text[i] = html_file_lines[i];
	}

	return new_file;
}

int main(int argc, char** argv) {
	FileInfo file;
	if (argc > 1) {
		file = get_file_lines(argv[1]);
	} else {
		file = get_stdin_lines(stdin);
	}
	file = build_html(file);
	file = build_headers(file);
	file = build_lists(file);
	file = build_paras(file);
	file = build_emphasis(file);
	file = build_hyperlinks(file);

	for (int i = 0; i < file.number_of_lines; i++) {
		printf("%s", file.line_text[i]);
	}

	exit(EXIT_SUCCESS);
}
