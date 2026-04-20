#include "markdown.h"

FileInfo get_file_lines(char* file_name) {
	FILE* fp;
	char* buffer = NULL;
	size_t bufsize = 0;
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
	buffer = malloc(bufsize);

	// We go through the file AGAIN to actually get the content this time
	fp = fopen(file_name, "r");
	if (fp == NULL) exit(EXIT_FAILURE);
	int i = 0;
	while ((read = getline(&buffer, &bufsize, fp)) != -1) {
		size_t line_length = strlen(buffer) + 1;
		file_text[i] = malloc(1 + strlen(buffer));
		strlcpy(file_text[i], buffer, line_length);
		i++;
	}

	fclose(fp);
	free(buffer);

	FileInfo file = {
		.number_of_lines = lines_total,
		.total_len = len_total,
		.line_text = malloc(len_total * lines_total),
	};

	for (int i = 0; i < file.number_of_lines; i++) {
		size_t line_length = strlen(file_text[i]) + 20;
		file.line_text[i] = malloc(sizeof(file.line_text[i]) * line_length);
		strlcpy(file.line_text[i], file_text[i], line_length);
	}

	buffer = NULL;

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
	size_t bufsize;
	ssize_t read;

	buffer = malloc(bufsize);

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

FileInfo build_headers(FileInfo file) {
	char* buffer;
	char* new_header;

	for (int i = 0; i < file.number_of_lines; i++) {
		if (strncmp(file.line_text[i], MD_HEADING4, 4) == 0) {
		// H4
			size_t new_line_length = strlen(file.line_text[i] + 1);
			new_line_length += strlen(H4_START);
			new_line_length += strlen(H4_END);

			buffer = malloc(1 + strlen(file.line_text[i]));
			new_header = malloc(new_line_length);

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 4];
			}

			strlcpy(new_header, "", sizeof(new_header));
			strlcat(new_header, H4_START, new_line_length);
			strlcat(new_header, buffer, new_line_length);
			strlcat(new_header, H4_END, new_line_length);
			file.line_text[i] = realloc(file.line_text[i], new_line_length);
			strlcpy(file.line_text[i], new_header, new_line_length);

			//free(buffer);
			//free(new_header);
		} else if (strncmp(file.line_text[i], MD_HEADING3, 3) == 0) {
		// H3
			size_t new_line_length = strlen(file.line_text[i] + 1);
			new_line_length += strlen(H3_START);
			new_line_length += strlen(H3_END);

			buffer = malloc(1 + strlen(file.line_text[i]));
			new_header = malloc(new_line_length);

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 3];
			}

			strlcpy(new_header, "", sizeof(new_header));
			strlcat(new_header, H3_START, new_line_length);
			strlcat(new_header, buffer, new_line_length);
			strlcat(new_header, H3_END, new_line_length);
			file.line_text[i] = realloc(file.line_text[i], new_line_length);
			strlcpy(file.line_text[i], new_header, new_line_length);

			//free(buffer);
			//free(new_header);
		} else if (strncmp(file.line_text[i], MD_HEADING2, 2) == 0) {
		// H2
			size_t new_line_length = strlen(file.line_text[i] + 1);
			new_line_length += strlen(H2_START);
			new_line_length += strlen(H2_END);

			buffer = malloc(1 + strlen(file.line_text[i]));
			new_header = malloc(new_line_length);

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 2];
			}

			strlcpy(new_header, "", sizeof(new_header));
			strlcat(new_header, H2_START, new_line_length);
			strlcat(new_header, buffer, new_line_length);
			strlcat(new_header, H2_END, new_line_length);
			file.line_text[i] = realloc(file.line_text[i], new_line_length);
			strlcpy(file.line_text[i], new_header, new_line_length);

			//free(buffer);
			//free(new_header);
		} else if (strncmp(file.line_text[i], MD_HEADING1, 1) == 0) {
		// H1
			size_t new_line_length = strlen(file.line_text[i] + 1);
			new_line_length += strlen(H1_START);
			new_line_length += strlen(H1_END);

			buffer = malloc(1 + strlen(file.line_text[i]));
			new_header = malloc(new_line_length);

			for (int j = 0; j < strlen(file.line_text[i]); j++) {
				buffer[j]  = file.line_text[i][j + 1];
			}

			strlcpy(new_header, "", sizeof(new_header));
			strlcat(new_header, H1_START, new_line_length);
			strlcat(new_header, buffer, new_line_length);
			strlcat(new_header, H1_END, new_line_length);
			file.line_text[i] = realloc(file.line_text[i], new_line_length);
			strlcpy(file.line_text[i], new_header, new_line_length);

			//free(buffer);
			//free(new_header);
		}
	}

	return file;
}

FileInfo build_lists(FileInfo file) {
	char* buffer;
	char* list_item;
	int count = 0;
	int emer_count = 100;

	for (int i = 0; i < file.number_of_lines; i++) {
		if (file.line_text[i][0] == '*' && file.line_text[i][1] == ' ') {
			count++;
		}
	}

	// Convert the individual list items
	while (count > 1) {
		emer_count -= 1;
		if (emer_count == 0) {
			fprintf(stderr, "Error: list loop failure\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < file.number_of_lines; i++) {
			if (strncmp(file.line_text[i], MD_LIST, 2) == 0) {

				int line_length = strlen(file.line_text[i]) + 1;
				int new_line_length = line_length;
				new_line_length += strlen(LIST_ITEM_START) + 1;
				new_line_length += strlen(LIST_ITEM_END) + 1;

				buffer = malloc(line_length);
				list_item = malloc(new_line_length);

				for (int j = 0; j < strlen(file.line_text[i]); j++) {
					buffer[j]  = file.line_text[i][j + 2];
				}

				strncat(list_item, LIST_ITEM_START, strlen(LIST_ITEM_START) + 1);
				strncat(list_item, buffer, line_length);
				strncat(list_item, LIST_ITEM_END, strlen(LIST_ITEM_END) + 1);
				strncpy(file.line_text[i], list_item, new_line_length);

				buffer = NULL;
				list_item = NULL;

				//free(buffer);
				//free(list_item);

				count--;
			}
		}
	}

	// Add the unordered list starting tag before the list items
	for (int i = 0; i < file.number_of_lines; i++) {
		if (!strstr(file.line_text[i], LIST_ITEM_END)) {
			if (i == file.number_of_lines - 1) break;
			if (strstr(file.line_text[i + 1], LIST_ITEM_START)) {
				buffer = malloc(strlen(file.line_text[i]));
				list_item = malloc(1000);

				strcpy(buffer, file.line_text[i]);

				strcat(list_item, LIST_START);
				strcat(list_item, buffer);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;

				//free(buffer);
				//free(list_item);
			}
		}
	}

	// Add the unordered list ending tag after the list items
	for (int i = 0; i < file.number_of_lines; i++) {
		if (strstr(file.line_text[i], LIST_ITEM_START)) {
			if (i == file.number_of_lines - 2) {
				buffer = malloc(strlen(file.line_text[i]));
				list_item = malloc(1000);

				strcpy(buffer, file.line_text[i]);

				strcat(list_item, buffer);
				strcat(list_item, LIST_END);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;

				//free(buffer);
				//free(list_item);

				break;
			}

			int index = strlen(file.line_text[i + 1]);
			char* last_five = &file.line_text[i + 1][index - 5];
			if (strncmp(last_five, LIST_ITEM_END, 5)) {
				buffer = malloc(strlen(file.line_text[i]));
				list_item = malloc(1000);

				strcpy(buffer, file.line_text[i]);

				strcat(list_item, buffer);
				strcat(list_item, LIST_END);
				strcpy(file.line_text[i], list_item);

				buffer = NULL;
				list_item = NULL;

				//free(buffer);
				//free(list_item);
			}
		}
	}

	return file;
}

FileInfo build_paras(FileInfo file) {
	char* new_line = NULL;

	for (int i = 0; i < file.number_of_lines; i++) {
		size_t line_length = strlen(file.line_text[i]) + 1;
		line_length += strlen(PARA_START) + 1;
		line_length += strlen(PARA_END) + 1;
		if (strstr(file.line_text[i], "  ")) {
			new_line = malloc(line_length);
			strlcpy(new_line, "", sizeof(new_line));
			strlcat(new_line, PARA_START, line_length);
			strlcat(new_line, file.line_text[i], line_length);
			strlcat(new_line, PARA_END, line_length);
			file.line_text[i] = realloc(file.line_text[i], line_length);
			strlcpy(file.line_text[i], new_line, line_length);

			new_line = NULL;
			//free(new_line);
		}
	}

	return file;
}

// This works but it's very ugly and I hate it
FileInfo build_emphasis(FileInfo file) {
	char* search_buffer = NULL;
	char* buffer = NULL;
	char* before_text = NULL;
	char* after_text = NULL;
	char* new_line = NULL;

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
			fprintf(stderr, "Error: emphasis loop failure\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < file.number_of_lines; i++) {
			search_buffer = malloc(2000);
			if ((search_buffer = strstr(file.line_text[i], MD_BOLDITALIC)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				if (!strncmp(&file.line_text[i][index1 + 2], "*", 1)) {
					search_buffer = malloc(line_length);
					buffer = malloc(line_length);
					before_text = malloc(line_length - index1);

					if ((search_buffer = strstr(file.line_text[i] + index1 + 2, MD_BOLDITALIC)) == NULL) continue;
					index2 = search_buffer - file.line_text[i];
					after_text = malloc(line_length - index2);
					strncpy(buffer, file.line_text[i] + index1 + 3, index2 - index1 - 2);

					size_t new_line_length = line_length + 1;
					new_line_length += strlen(BOLDITALIC_START) + 1;
					new_line_length += strlen(BOLDITALIC_END) + 1;
					new_line = malloc(new_line_length);

					strncpy(before_text, file.line_text[i], index1);
					strncpy(after_text, file.line_text[i] + index2 + 3, line_length - index2);

					strlcat(after_text, "", sizeof(after_text));
					strcat(new_line, before_text);
					strcat(new_line, BOLDITALIC_START);
					strcat(new_line, buffer);
					strcat(new_line, BOLDITALIC_END);
					strcat(new_line, after_text);

					file.line_text[i] = realloc(file.line_text[i], new_line_length);
					strcpy(file.line_text[i], new_line);

					search_buffer = NULL;
					buffer = NULL;
					before_text = NULL;
					after_text = NULL;
					new_line = NULL;

					//free(search_buffer);
					//free(buffer);
					//free(before_text);
					//free(after_text);
					//free(new_line);

					count -= 6;
				}
			}

			if ((search_buffer = strstr(file.line_text[i], MD_BOLD)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				 if (!strncmp(&file.line_text[i][index1 + 1], "*", 1)) {
					search_buffer = malloc(line_length) + 1;
					buffer = malloc(line_length) + 1;
					before_text = malloc(index1 + 1);

					if ((search_buffer = strstr(file.line_text[i] + index1 + 2, MD_BOLD)) == NULL) continue;
					index2 = search_buffer - file.line_text[i];
					after_text = malloc(line_length - index2);

					strncpy(buffer, file.line_text[i] + index1 + 2, index2 - index1 - 2);
					strlcpy(before_text, "", index1 + 1);
					strncat(before_text, file.line_text[i], index1);
					strlcpy(after_text, file.line_text[i] + index2 + 2, index2);

					size_t new_line_length = line_length + 1;
					new_line_length += strlen(BOLD_START) + 1;
					new_line_length += strlen(BOLD_END) + 1;
					new_line = malloc(new_line_length);

					strlcpy(new_line, "", sizeof(new_line));
					strlcat(new_line, before_text, new_line_length);
					strlcat(new_line, BOLD_START, new_line_length);
					strlcat(new_line, buffer, new_line_length);
					strlcat(new_line, BOLD_END, new_line_length);
					strlcat(new_line, after_text, new_line_length);

					file.line_text[i] = realloc(file.line_text[i], new_line_length);
					strlcpy(file.line_text[i], new_line, new_line_length);

					search_buffer = NULL;
					buffer = NULL;
					before_text = NULL;
					after_text = NULL;
					new_line = NULL;

					//free(search_buffer);
					//free(buffer);
					//free(before_text);
					//free(after_text);
					//free(new_line);

					count -= 4;
				}
			}

			if ((search_buffer = strstr(file.line_text[i], MD_ITALIC)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				if (!strncmp(&file.line_text[i][index1], "*", 1)) {
					search_buffer = malloc(line_length);
					buffer = malloc(line_length);
					before_text = malloc(index1 + 1);

					if ((search_buffer = strstr(file.line_text[i] + index1 + 1, MD_ITALIC)) == NULL) continue;
					index2 = search_buffer - file.line_text[i];
					after_text = malloc(line_length - index2);

					strncpy(buffer, file.line_text[i] + index1 + 1, index2 - index1 - 1);
					strlcpy(before_text, "", index1 + 1);
					strncpy(before_text, file.line_text[i], index1);
					strlcpy(after_text, file.line_text[i] + index2 + 1, line_length - index2);

					size_t new_line_length = line_length + 1;
					new_line_length += strlen(BOLD_START) + 1;
					new_line_length += strlen(BOLD_END) + 1;
					new_line = malloc(new_line_length);

					strlcat(new_line, "", sizeof(new_line));
					strlcat(new_line, before_text, new_line_length);
					strlcat(new_line, ITALIC_START, new_line_length);
					strlcat(new_line, buffer, new_line_length);
					strlcat(new_line, ITALIC_END, new_line_length);
					strlcat(new_line, after_text, new_line_length);

					file.line_text[i] = realloc(file.line_text[i], new_line_length);
					strlcpy(file.line_text[i], new_line, new_line_length);

					search_buffer = NULL;
					buffer = NULL;
					before_text = NULL;
					after_text = NULL;
					new_line = NULL;

					//free(search_buffer);
					//free(buffer);
					//free(before_text);
					//free(after_text);
					//free(new_line);

					count -= 2;
				}
			}
		}
	}

	return file;
}

FileInfo build_hyperlinks(FileInfo file) {
	char* search_buffer; 
	char* urltext_buffer;
	char* url_buffer;
	char* before_text;
	char* after_text;
	char* new_line;

	int count = 0;
	int len = 0;
	int line_length = 0;
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	int emer_count = 100;

	for (int i = 0; i < file.number_of_lines; i++) {
		len = strlen(file.line_text[i]);
		for (int j = 0; j < len; j++) {
			if (file.line_text[i][j] == '[') count++;
		}
	}

	while (count > 1) {
		emer_count -= 1;
		if (emer_count == 0) { 
			fprintf(stderr, "Error: hyperlink loop failure\n");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < file.number_of_lines; i++) {
			search_buffer = malloc(strlen(file.line_text[i])); 
			if ((search_buffer = strstr(file.line_text[i], MD_HYPERLINK_START)) != NULL) {
				line_length = strlen(file.line_text[i]);
				index1 = search_buffer - file.line_text[i];
				search_buffer = NULL;
				search_buffer = malloc(line_length);
				urltext_buffer = malloc(line_length);
				url_buffer = malloc(line_length);
				before_text = malloc(line_length);
				after_text = malloc(line_length);

				if ((search_buffer = strstr(file.line_text[i] + index1 + 1, MD_HYPERLINK_MIDDLE)) == NULL) continue;
				index2 = search_buffer - file.line_text[i];
				strncpy(urltext_buffer, file.line_text[i] + index1 + 1, index2 - index1 - 1);
				if ((search_buffer = strstr(file.line_text[i] + index2 + 1, MD_HYPERLINK_END)) == NULL) continue;
				index3 = search_buffer - file.line_text[i];
				strncpy(url_buffer, file.line_text[i] + index2 + 2, index3 - index2 - 2);

				strncpy(before_text, file.line_text[i], index1);
				strncpy(after_text, file.line_text[i] + index3 + 1, line_length - index2);

				line_length += strlen(urltext_buffer) + strlen(url_buffer);
				line_length += strlen(HYPERLINK_START) + strlen(HYPERLINK_MIDDLE) + strlen(HYPERLINK_END);
				new_line = malloc(line_length);
				strncat(new_line, before_text, index1);
				strncat(new_line, HYPERLINK_START, strlen(HYPERLINK_START) + 1);

				strncat(new_line, url_buffer, strlen(url_buffer) + 1);
				strncat(new_line, HYPERLINK_MIDDLE, strlen(HYPERLINK_MIDDLE) + 1);
				strncat(new_line, urltext_buffer, strlen(urltext_buffer) + 1);
				strncat(new_line, HYPERLINK_END, strlen(HYPERLINK_END) + 1);

				strncat(new_line, after_text, strlen(after_text) + 1);
				strncpy(file.line_text[i], new_line, strlen(new_line) + 1);

				urltext_buffer = NULL;
				url_buffer = NULL;
				before_text = NULL;
				after_text = NULL;
				new_line = NULL;

				//free(search_buffer);
				//free(url_buffer);
				//free(urltext_buffer);
				//free(before_text);
				//free(after_text);
				//free(new_line);

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
	size_t line_length = strlen(HTML_START) + 1;

	html_file_lines[0] = malloc(line_length);
	if (html_file_lines[0] == NULL) {
		fprintf(stderr, "build_html 1: malloc error\n");
		exit(EXIT_FAILURE);
	}

	strlcpy(html_file_lines[0], HTML_START, line_length);
	line_length = strlen(HTML_END) + 1;
	html_file_lines[new_line_total - 1] = malloc(line_length);
	if (html_file_lines[new_line_total - 1] == NULL) {
		fprintf(stderr, "build_html 2: malloc error\n");
		exit(EXIT_FAILURE);
	}

	strlcpy(html_file_lines[new_line_total - 1], HTML_END, line_length);

	for (int i = 1; i < new_line_total; i++) {
		line_length = strlen(file.line_text[i - 1]) + 1;
		html_file_lines[i] = malloc(line_length);
		if (html_file_lines[i] == NULL) {
			fprintf(stderr, "build_html 3: malloc error\n");
			exit(EXIT_FAILURE);
		}
		strlcpy(html_file_lines[i], file.line_text[i - 1], line_length);
		if (i == file.number_of_lines) break;
	}

	FileInfo new_file = {
		.number_of_lines = new_line_total,
		.total_len = file.total_len + 10,
		.line_text = malloc(file.total_len * new_line_total),
	};

	for (int i = 0; i < new_file.number_of_lines; i++) {
		line_length = strlen(html_file_lines[i]) + 20;
		new_file.line_text[i] = malloc(line_length);
		if (new_file.line_text[i] == NULL) {
			fprintf(stderr, "build_html 4: malloc error\n");
			exit(EXIT_FAILURE);
		}
		strlcpy(new_file.line_text[i], html_file_lines[i], line_length);
	}

	return new_file;
}

int convert_to_html(int argc, char** argv) {
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

	return EXIT_SUCCESS;
}
