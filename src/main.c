#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlstring.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

long get_elapsed_ms(struct timeval *start, struct timeval *end) {
  long seconds = end->tv_sec - start->tv_sec;
  long microseconds = end->tv_usec - start->tv_usec;

  return (seconds * 1000) + (microseconds / 1000);
}

int utf8_char_len(unsigned char c) {
  if (c < 0x80)
    return 1; // ASCII
  else if ((c & 0xE0) == 0xC0)
    return 2; // 2 bytes
  else if ((c & 0xF0) == 0xE0)
    return 3; // 3 bytes (한글)
  else if ((c & 0xF8) == 0xF0)
    return 4; // 4 bytes (이모지)
  return 1;
}

int print_with_delay(const char *string, int idx, int delay_ms) {
  int len = utf8_char_len((unsigned char)string[idx]);
  fwrite(&string[idx], 1, len, stdout);
  fflush(stdout);

  usleep(delay_ms * 1000);
  return idx + len;
}

int parse_to_ms(const char *time) {
  int m = 0, s = 0, ms = 0;

  if (strchr((const char *)time, ':'))
    sscanf(time, "%d:%d.%d", &m, &s, &ms);
  else
    sscanf(time, "%d.%d", &s, &ms);

  return m * 60 * 1000 + s * 1000 + ms;
}

void sync_and_print(xmlNodePtr node, struct timeval *start) {
  int start_ms = 0, end_ms = 0;

  xmlChar *begin = xmlGetProp(node, (const xmlChar *)"begin");
  if (begin != NULL) {
    start_ms = parse_to_ms((const char *)begin);
    xmlFree(begin);
  }

  xmlChar *end = xmlGetProp(node, (const xmlChar *)"end");
  if (end != NULL) {
    end_ms = parse_to_ms((const char *)end);
    xmlFree(end);
  }

  xmlChar *inner_text = xmlNodeListGetString(node->doc, node->xmlChildrenNode, 1);

  struct timeval now;
  long elapsed_ms;

  do {
    gettimeofday(&now, NULL);
    elapsed_ms = get_elapsed_ms(start, &now);
  } while (start_ms > elapsed_ms);

  if (inner_text != NULL) {
    int i = 0;
    int ms_per_text = (end_ms - start_ms) / strlen((const char *)inner_text);

    while (inner_text[i] != '\0') {
      i = print_with_delay((const char *)inner_text, i, ms_per_text);
    }

    xmlFree(inner_text);
  } else {
    printf("\n");
  }
}

int main(void) {
  xmlDocPtr doc = xmlReadFile("assets/외톨이.xml", "UTF-8", XML_PARSE_NOBLANKS);
  xmlNodePtr root_node = xmlDocGetRootElement(doc);

  if (!root_node) {
    xmlFreeDoc(doc);
    return -1;
  }

  xmlNodePtr body_node = root_node->children;
  while (body_node != NULL) {
    if (body_node->type == XML_ELEMENT_NODE &&
        xmlStrcmp(body_node->name, (const xmlChar *)"body") == 0)
      break;
    body_node = body_node->next;
  }
  if (body_node == NULL)
    return -1;

  xmlChar *dur_value = xmlGetProp(body_node, (const xmlChar *)"dur");
  if (dur_value != NULL) {
    printf("Body duration (dur): %s\n", dur_value);
    xmlFree(dur_value);
  }

  struct timeval start;
  gettimeofday(&start, NULL);

  for (xmlNodePtr div = body_node->children; div != NULL; div = div->next) {
    if (xmlStrcmp(div->name, (const xmlChar *)"div") == 0) {
      for (xmlNodePtr p = div->children; p != NULL; p = p->next) {
        if (xmlStrcmp(p->name, (const xmlChar *)"p") == 0) {
          for (xmlNodePtr span = p->children; span != NULL; span = span->next) {
            sync_and_print(span, &start);
          }
        } else {
          sync_and_print(p, &start);
        }
        printf("\n");
      }
    }
  }

  xmlFreeDoc(doc);

  return 0;
}
