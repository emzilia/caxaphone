#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
	size_t	total_len;
	char**	line_text;
} FileInfo;


FileInfo get_file_lines(char* file_name);
FileInfo get_stdin_lines(FILE* input);
FileInfo build_headers(FileInfo file);
FileInfo build_lists(FileInfo file);
FileInfo build_paras(FileInfo file);
FileInfo build_emphasis(FileInfo file);
FileInfo build_hyperlinks(FileInfo file);
FileInfo build_html(FileInfo file);
int convert_to_html(int argc, char** argv);
