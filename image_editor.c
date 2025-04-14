//Copyright Grigore Mihnea - Andrei

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define NOSEL -1000

typedef struct {
	int r, g, b;
} color;

typedef struct {
	double r, g, b;
} color_double;

void image_load(char *image_name, int *type, int *l, int *h, bool *loaded);
int select_zone(const int col, const int line, int *x1, int *y1, int *x2,
				int *y2);
void select_all(const bool ok, const int col, const int line, int *x1,
				int *y1, int *x2, int *y2);
void alloc_color(const int l, const int h, color ***picture);
void alloc_nocolor(const int col, const int line, int ***picture);
void alloc_double(const int col, const int line, color_double ***picture);
void free_matrix(const int type, const int line, color ***picture,
				 int ***monochrome);
void free_color(const int line, color **picture);
void free_nocolor(const int line, int **picture);
void read_color(const int col, const int line, const int type,
				color **picture, char *image_name);
void free_double(const int line, color_double **picture);
void read_nocolor(const int col, const int line, const int type,
				  int **picture, char *image_name);
int crop_color(int *col, int *line, color ***picture,
			   const int x1, const int y1, const int x2, const int y2);
int crop_nocolor(int *col, int *line, int ***picture,
				 const int x1, const int y1, const int x2, const int y2);
void rotate(const bool loaded, int *full_col, int *full_line, int *x1,
			int *x2, int *y1, int *y2, int type, color ***picture,
			int ***monochrome);
void save(int type, const int col, const int line, char save_details[101],
		  int **monochrome, color **picture);
void swap(int *a, int *b);
void edge_apply(const int col, const int line, const int x1, const int x2,
				const int y1, const int y2, color ***picture);
void sharpen_apply(const int col, const int line, const int x1, const int x2,
				   const int y1, const int y2, color ***picture);
int apply(const bool loaded, const int type, const int col, const int line,
		  const int x1, const int x2, const int y1, const int y2,
		  color ***picture);
int apply_command(const char apply_type[51]);
void edge(int **a);
int clamp(int x);
void get_line(void);
void save_values(const int x1, const int x2, const int y1, const int y2,
				 int *old_x1, int *old_x2, int *old_y1, int *old_y2);
void histogram(const int loaded, const int type, const int col, const int line,
			   int **picture);
int equalize(const int loaded, const int type, const int col, const int line,
			 int ***picture);

void zone(int l, int h, int *x1, int *x2, int *y1, int *y2, int old_x1);
void check_invalid(const bool got_command, int l, int h,
				   int *x1, int *x2, int *y1, int *y2);
void check_loaded(bool loaded, int type, const int h,
				  color ***picture, int ***monochrome);
void not_loaded(void);
void final_check(char command[10], bool selected, int old_x1, int old_x2,
				 int old_y1, int old_y2, int *x1, int *x2, int *y1, int *y2,
				 bool loaded, int type, int h, color ***picture,
				 int ***monochrome);
void load_handle(char image_name[101], int type, int l, int h, int *x1,
				 int *x2, int *y1, int *y2, int ***monochrome,
				 color ***picture);
void select_handle(int l, int h, int *x1, int *x2, int *y1,
				   int *y2, int old_x1);

int main(void)
{
	char command[10], image_name[101];
	int x1 = NOSEL, y1 = NOSEL, x2 = NOSEL, y2 = NOSEL;
	int type, l = 0, h = 0, **monochrome = NULL, old_x1, old_x2, old_y1, old_y2;
	bool loaded = 0, got_command;
	color **picture = NULL;
	scanf("%s", command);
	while (strcmp(command, "EXIT")) {
		if (command[0] == '#') { // comments
			get_line();
		} else {
			got_command = 0;
			save_values(x1, x2, y1, y2, &old_x1, &old_x2, &old_y1, &old_y2);
			if (!strcmp(command, "LOAD")) {
				got_command = 1;
				image_load(image_name, &type, &l, &h, &loaded); //load the image
				if (loaded)
					load_handle(image_name, type, l, h, &x1, &x2, &y1, &y2,
								&monochrome, &picture);
			}
			if (!strcmp(command, "SELECT")) {
				got_command = 1;
				if (!loaded)
					not_loaded();
				else
					select_handle(l, h, &x1, &x2, &y1, &y2, old_x1);
			}
			if (!strcmp(command, "CROP")) {
				got_command = 1;
				if (!loaded) {
					not_loaded();
				} else if (type % 3 == 0) { // RGB
					if (crop_color(&l, &h, &picture, x1, y1, x2, y2))
						select_all(0, l, h, &x1, &y1, &x2, &y2);
					} else if (crop_nocolor(&l, &h, &monochrome,
											x1, y1, x2, y2))
						select_all(0, l, h, &x1, &y1, &x2, &y2);
			}
			if (!strcmp(command, "SAVE")) {
				got_command = 1;
				if (!loaded) {
					not_loaded();
				} else {
					char save_details[101], buffer;
					scanf("%c", &buffer);
					fgets(save_details, 101, stdin);
					save_details[strlen(save_details) - 1] = '\0'; // delete \n
					save(type, l, h, save_details, monochrome, picture);
				}
			}
			if ((!strcmp(command, "ROTATE"))) {
				got_command = 1;
				rotate(loaded, &l, &h, &x1, &x2, &y1, &y2,
					   type, &picture, &monochrome);
			}
			if (!strcmp(command, "APPLY")) {
				got_command = 1;
				apply(loaded, type, l, h, x1, x2, y1, y2, &picture);
			}
			if (!strcmp(command, "HISTOGRAM")) {
				got_command = 1;
				histogram(loaded, type, l, h, monochrome);
				select_all(0, l, h, &x1, &y1, &x2, &y2);
			}
			if (!strcmp(command, "EQUALIZE")) {
				got_command = 1;
				equalize(loaded, type, l, h, &monochrome);
			}
			check_invalid(got_command, l, h, &x1, &x2, &y1, &y2);
		}
		bool selected = 0;
		if (!strcmp(command, "SELECT"))
			selected = 1;
		scanf("%s", command);
		final_check(command, selected, old_x1, old_x2, old_y1, old_y2, &x1,
					&x2, &y1, &y2, loaded, type, h, &picture, &monochrome);
	}
	check_loaded(loaded, type, h, &picture, &monochrome);
	return 0;
}

// handle the select_zone function
void zone(int l, int h, int *x1, int *x2, int *y1, int *y2, int old_x1)
{
	if (!select_zone(l, h, x1, y1, x2, y2))
		*x1 = old_x1; //get back to old selection
}

// print invalid command error
void check_invalid(const bool got_command, int l, int h,
				   int *x1, int *x2, int *y1, int *y2)
{
	if (!got_command) {
		printf("Invalid command\n");
		get_line();
		select_all(0, l, h, x1, y1, x2, y2);
	}
}

// verify if the image is still loaded and free it
void check_loaded(bool loaded, int type, const int h,
				  color ***picture, int ***monochrome)
{
	if (!loaded)
		printf("No image loaded\n");
	else
		free_matrix(type, h, picture, monochrome); // free all
}

// print no image loaded error
void not_loaded(void)
{
	printf("No image loaded\n");
	get_line();
}

// make the final check on memory allocation and free memory if necessary
void final_check(char command[10], bool selected, int old_x1, int old_x2,
				 int old_y1, int old_y2, int *x1, int *x2, int *y1, int *y2,
				 bool loaded, int type, int h, color ***picture,
				 int ***monochrome)
{
	if (!strcmp(command, "SAVE") && selected)
		select_all(0, old_x2 - old_x1, old_y2 - old_y1, x1, y1, x2, y2);
	if (!strcmp(command, "LOAD") && loaded) {
		free_matrix(type, h, picture, monochrome);
		loaded = 0;
	}
}

// handle the load command taking into account the image type
void load_handle(char image_name[101], int type, int l, int h, int *x1,
				 int *x2, int *y1, int *y2, int ***monochrome,
				 color ***picture)
{
	select_all(0, l, h, x1, y1, x2, y2);
	if (type == 3 || type == 6) { // is color type rgb
		alloc_color(l, h, picture); // calloc for rgb picture
		read_color(l, h, type, *picture, image_name); //read rgb
		} else if (type == 1 || type == 2 || type == 4 || type == 5) {
			alloc_nocolor(l, h, monochrome); //calloc
			read_nocolor(l, h, type, *monochrome, image_name);
	}
}

// handle the select command and print error messages if necessary
void select_handle(int l, int h, int *x1, int *x2, int *y1,
				   int *y2, int old_x1)
{
	char space, select_type[100];
	scanf("%c%s", &space, select_type);
	if (select_type[0] == '-') { //negative
		printf("Invalid set of coordinates\n");
		get_line();
	} else if (select_type[0] >= '0' &&
		select_type[0] <= '9') {
		*x1 = atoi(select_type); //first number
		zone(l, h, x1, x2, y1, y2, old_x1);
		} else if (!(strcmp(select_type, "ALL")))
			select_all(1, l, h, x1, y1, x2, y2);
}

// save the previus selection
void save_values(const int x1, const int x2, const int y1, const int y2,
				 int *old_x1, int *old_x2, int *old_y1, int *old_y2)
{
	*old_x1 = x1;
	*old_x2 = x2;
	*old_y1 = y1;
	*old_y2 = y2;
}

// read the rest of the line
void get_line(void)
{
	char buffer[51];
	fgets(buffer, 51, stdin);
}

//swap values
void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// load the images or error
void image_load(char *image_name, int *type, int *l, int *h, bool *loaded)
{
	int max;
	char buffer, letter, number;
	scanf("%c", &buffer); // read the space after command
	fgets(image_name, 101, stdin); // read the image name
	image_name[strlen(image_name) - 1] = '\0'; // delete \n
	FILE *image = fopen(image_name, "r");
	if (!image) {
		printf("Failed to load %s\n", image_name);
		*loaded = 0; // mark as not loaded
		return;
	}
	printf("Loaded %s\n", image_name);
	*loaded = 1; // mark as loaded
	fscanf(image, "%c%c", &letter, &number);
	*type = number - '0'; // get the image type as int
	fscanf(image, "%d%d%d", l, h, &max);
	fclose(image); // close the file
}

// read the arguments for select or error
int read_select_arg(void)
{
	char m[10], buffer;
	scanf("%c", &buffer);
	if (buffer == '\n') { // get the blank characters
		printf("Invalid command\n");
		return NOSEL;
	}
	scanf("%s", m);
	if (m[0] >= '0' && m[0] <= '9')
		return atoi(m); // if is number return it as int
	printf("Invalid command\n");
	return NOSEL;
}

// select a zone in image
int select_zone(const int col, const int line, int *x1, int *y1,
				int *x2, int *y2)
{
	int a, b, c;
	a = read_select_arg();
	if (a == NOSEL)
		return 0;

	b = read_select_arg();
	if (b == NOSEL)
		return 0;

	c = read_select_arg();
	if (c == NOSEL)
		return 0;

	// get the correct order of arguments
	if (*x1 > b)
		swap(x1, &b);

	if (a > c)
		swap(&a, &c);

	// handle the errors
	if (a < 0 || b < 0 || c < 0 || *x1 < 0 || *x1 > col || b > col ||
	    a > line || c > line) {
		printf("Invalid set of coordinates\n");
		get_line();
		return 0;
	}
	if (a == b && b == c && c == *x1) {
		printf("Invalid set of coordinates\n");
		get_line();
		return 0;
	}
	if (a == c || *x1 == b) {
		printf("Invalid set of coordinates\n");
		get_line();
		return 0;
	}

	*y1 = a;
	*x2 = b;
	*y2 = c;
	printf("Selected %d %d %d %d\n", *x1, a, b, c);
	return 1;
}

// select all image
void select_all(const bool ok, const int col, const int line, int *x1,
				int *y1, int *x2, int *y2)
{
	*x1 = *y1 = 0;
	*x2 = col;
	*y2 = line;
	if (ok)
		printf("Selected ALL\n");
}

// alloc the color image
void alloc_color(const int col, const int line, color ***picture)
{
	*picture = (color **)calloc(line, sizeof(color *));
	if (!(*picture)) {
		printf("Error on memory allocation for rows\n");
		return;
	}

	for (int i = 0; i < line; i++) {
		(*picture)[i] = (color *)calloc(col, sizeof(color));
		if (!(*picture)[i]) {
			printf("Error on memory allocation for columns at row %d\n", i);
			for (int k = 0; k < i; k++) {
				free((*picture)[k]);
			}
			free(*picture);
			*picture = NULL;
			return;
		}
	}
}

// alloc the black and white image
void alloc_nocolor(const int col, const int line, int ***picture)
{
	*picture = (int **)calloc(line, sizeof(int *));
	if (!(*picture)) {
		printf("Error at calloc on lines.\n");
		return;
	}
	for (int i = 0; i < line; i++) {
		(*picture)[i] = (int *)calloc(col, sizeof(int));
		if (!(*picture)[i]) {
			printf("Error at calloc for columns at row %d\n", i);
			for (int k = 0; k < i; k++) {
				free((*picture)[k]);
			}
			free(*picture);
			*picture = NULL;
			return;
		}
	}
}

// alloc the double matrix
void alloc_double(const int col, const int line, color_double ***picture)
{
	*picture = (color_double **)calloc(line, sizeof(color_double *));
	if (!(*picture)) {
		printf("Error on memory allocation for rows\n");
		return;
	}
	for (int i = 0; i < line; i++) {
		(*picture)[i] = (color_double *)calloc(col, sizeof(color_double));
		if (!(*picture)[i]) {
			printf("Error on memory allocation for columns at row %d\n", i);
			for (int k = 0; k < i; k++) {
				free((*picture)[k]);
			}
			free(*picture);
			*picture = NULL;
			return;
		}
	}
}

// free the color image
void free_color(const int line, color **picture)
{
	for (int i = 0; i < line; i++)
		free(picture[i]);
	free(picture);
}

// free the black and white matrix
void free_nocolor(const int line, int **picture)
{
	for (int i = 0; i < line; i++)
		free(picture[i]);
	free(picture);
}

// free the double matrix
void free_double(const int line, color_double **picture)
{
	for (int i = 0; i < line; i++)
		free(picture[i]);
	free(picture);
}

// final free for all still allocated images
void free_matrix(const int type, const int line, color ***picture,
				 int ***monochrome)
{
	if ((type == 3 || type == 6) && *picture) {
		free_color(line, *picture);
		*picture = NULL;
	} else if ((type == 1 || type == 2 || type == 4 || type == 5) &&
			   *monochrome) {
		free_nocolor(line, *monochrome);
		*monochrome = NULL;
	}
}

// read the RGB pixels
void read_color(const int col, const int line, const int type,
				color **picture, char *image_name)
{
	FILE *image = fopen(image_name, "rb");
	if (!image)
		return;
	char format[3];
	int max_value = 255;
	fscanf(image, "%2s", format);
	if ((type == 3 && strcmp(format, "P3") != 0) ||
		(type == 6 && strcmp(format, "P6") != 0)) {
		fprintf(stderr, "Error: Invalid file format\n");
		fclose(image);
		return;
	}

	int img_col, img_line;
	fscanf(image, "%d %d", &img_col, &img_line);
	fscanf(image, "%d", &max_value);

	if (img_col != col || img_line != line) {
		fprintf(stderr, "Error: Image dimensions do not match\n");
		fclose(image);
		return;
	}

	fgetc(image);

	if (type == 3) {
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < col; j++) {
				fscanf(image, "%d %d %d",
					   &picture[i][j].r,
					   &picture[i][j].g,
					   &picture[i][j].b);
			}
		}
	} else if (type == 6) {
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < col; j++) {
				unsigned char r, g, b;
				fread(&r, sizeof(unsigned char), 1, image);
				fread(&g, sizeof(unsigned char), 1, image);
				fread(&b, sizeof(unsigned char), 1, image);

				picture[i][j].r = (int)r;
				picture[i][j].g = (int)g;
				picture[i][j].b = (int)b;
			}
		}
	}

	fclose(image);
}

// read the black and white pixels
void read_nocolor(const int col, const int line, const int type, int **picture,
				  char *image_name)
{
	FILE *image = fopen(image_name, "rb");
	if (!image)
		return;
	char format[3];
	int max_value = 255;
	fscanf(image, "%2s", format);
	if ((type == 1 && strcmp(format, "P1") != 0) ||
		(type == 2 && strcmp(format, "P2") != 0) ||
		(type == 4 && strcmp(format, "P4") != 0) ||
		(type == 5 && strcmp(format, "P5") != 0)) {
		fprintf(stderr, "Error: Invalid file format\n");
		fclose(image);
		return;
	}

	int img_col, img_line;
	fscanf(image, "%d %d", &img_col, &img_line);
	if (type == 2 || type == 5) {
		fscanf(image, "%d", &max_value);
	}

	if (img_col != col || img_line != line) {
		fprintf(stderr, "Error: Image dimensions do not match\n");
		fclose(image);
		return;
	}

	fgetc(image);

	if (type == 1 || type == 2) {
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < col; j++) {
				fscanf(image, "%d", &picture[i][j]);
			}
		}
	} else if (type == 4 || type == 5) {
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < col; j++) {
				unsigned char pixel;
				fread(&pixel, sizeof(unsigned char), 1, image);
				picture[i][j] = (int)pixel;
			}
		}
	}

	fclose(image);
}

// crop the color image
int crop_color(int *col, int *line, color ***picture,
			   const int x1, const int y1, const int x2, const int y2)
{
	if (x2 - x1 == *col && y2 - y1 == *line) {
		printf("Image cropped\n");
		return 0;
	}

	color **aux = NULL; // aux matrix to save the cropped values
	alloc_color(x2 - x1, y2 - y1, &aux);

	for (int i = y1, k = 0; i < y2; i++, k++)
		for (int j = x1, p = 0; j < x2; j++, p++) {
			aux[k][p].r = (*picture)[i][j].r;
			aux[k][p].g = (*picture)[i][j].g;
			aux[k][p].b = (*picture)[i][j].b;
		}

	free_color(*line, *picture);

	*line = y2 - y1;
	*col = x2 - x1;
	*picture = aux;

	printf("Image cropped\n");
	return 1;
}

// crop for black and white image
int crop_nocolor(int *col, int *line, int ***picture,
				 const int x1, const int y1, const int x2, const int y2)
{
	if (x2 - x1 == *col && y2 - y1 == *line) {
		printf("Image cropped\n");
		return 0;
	}

	int **aux = NULL;
	alloc_nocolor(x2 - x1, y2 - y1, &aux);

	for (int i = y1, k = 0; i < y2; i++, k++)
		for (int j = x1, p = 0; j < x2; j++, p++) {
			aux[k][p] = (*picture)[i][j];
		}
	free_nocolor(*line, *picture);

	*line = y2 - y1;
	*col = x2 - x1;
	*picture = aux;

	printf("Image cropped\n");
	return 1;
}

// rotate image
void rotate(const bool loaded, int *full_col, int *full_line, int *x1,
			int *x2, int *y1, int *y2, int type,
			color ***picture, int ***monochrome)
{
	int col = *x2 - *x1, line = *y2 - *y1, angle;
	char buffer;
	scanf("%c%d", &buffer, &angle); // get the rotation angle
	if (!loaded) {
		printf("No image loaded\n");
		get_line();
		return;
	} else if (col != line && (*x2 - *x1 != *full_col ||
			   *y2 - *y1 != *full_line)) {
		printf("The selection must be square\n");
		return;
	} else if (abs(angle) == 360 || angle == 0) {
		printf("Rotated %d\n", angle);
		return;
	} else if (abs(angle) != 90 && abs(angle) != 180 && abs(angle) != 270) {
		printf("Unsupported rotation angle\n");
		return;
	}
	printf("Rotated %d\n", angle);
	if (angle < 0)
		angle += 360; // convert to positive
	if (col != line && *x2 - *x1 == *full_col && *y2 - *y1 == *full_line) {
		for (int k = 0; k < angle / 90; k++) {
			if (type == 3 || type == 6) { // type RGB
				color **rotated = NULL; // aux matrix
				alloc_color(line, col, &rotated);
				for (int i = 0, l = *y1; l < *y2; i++, l++)
					for (int j = 0, m = *x1; m < *x2; j++, m++)
						rotated[j][*y2 - 1 - l] = (*picture)[l][m];
				free_color(line, *picture);
				*picture = rotated;
				swap(x1, y1);
				swap(x2, y2);
				swap(&col, &line);
				swap(full_col, full_line);
			} else { // other types
				int **rotated = NULL; // aux matrix
				alloc_nocolor(line, col, &rotated);
				for (int i = 0, l = *y1; l < *y2; i++, l++)
					for (int j = 0, m = *x1; m < *x2; j++, m++)
						rotated[j][*y2 - 1 - l] = (*monochrome)[l][m];
				free_nocolor(line, *monochrome);
				*monochrome = rotated;
				swap(x1, y1);
				swap(x2, y2);
				swap(&col, &line);
				swap(full_col, full_line);
			}
		}
	} else {
		for (int k = 0; k < angle / 90; k++) {
			if (type == 3 || type == 6) {
				color **rotated = NULL;
				alloc_color(col, line, &rotated);
				for (int i = 0, l = *y1; l < *y2; i++, l++)
					for (int j = 0, m = *x1; m < *x2; j++, m++)
						rotated[j][line - 1 - i] = (*picture)[l][m];
				for (int i = 0, l = *y1; l < *y2; i++, l++)
					for (int j = 0, m = *x1; m < *x2; j++, m++)
						(*picture)[l][m] = rotated[i][j];
				free_color(line, rotated);
			} else {
				int **rotated = NULL;
				alloc_nocolor(col, line, &rotated);
				for (int i = 0, l = *y1; l < *y2; i++, l++) {
					for (int j = 0, m = *x1; m < *x2; j++, m++) {
						rotated[j][line - 1 - i] = (*monochrome)[l][m];
					}
				}
				for (int i = 0, l = *y1; l < *y2; i++, l++)
					for (int j = 0, m = *x1; m < *x2; j++, m++)
						(*monochrome)[l][m] = rotated[i][j];
				free_nocolor(line, rotated);
			}
		}
	}
}

// save the matrix
void save(int type, const int col, const int line, char save_details[101],
		  int **monochrome, color **picture)
{
	char file_name[101];
	int i;
	for (i = 0; (unsigned int)i < strlen(save_details); i++) {
		if (save_details[i] == ' ')
			break;
		file_name[i] = save_details[i];
	}
	file_name[i] = '\0';

	FILE *output = fopen(file_name, "w");

	if (strstr(save_details, "ascii")) { // print int
		if (type >= 4) // convert binary type to ascii
			type -= 3;
		fprintf(output, "P%d\n%d %d\n", type, col, line);
		if (type != 1 && type != 4)
			fprintf(output, "255\n");
		if (type == 3 || type == 6)
			for (int i = 0; i < line; i++)
				for (int j = 0; j < col; j++)
					fprintf(output, "%d %d %d ",
							picture[i][j].r, picture[i][j].g, picture[i][j].b);
		else {
			for (int i = 0; i < line; i++)
				for (int j = 0; j < col; j++)
					fprintf(output, "%d ", monochrome[i][j]);
		}
	} else { // print binary
		if (type < 4) // convert ascii type to binary
			type += 3;
		if (type != 1 && type != 4) {
			char header[] = {'P', (char)('0' + type), '\n'};
			fwrite(header, sizeof(char), 3, output);

			char dimensions[32];
			int dimensions_len = snprintf(dimensions, sizeof(dimensions),
										  "%d %d\n255\n", col, line);
			fwrite(dimensions, sizeof(char), dimensions_len, output);
		} else {
			char header[] = {'P', (char)('0' + type), '\n'};
			fwrite(header, sizeof(char), 3, output);

			char dimensions[32];
			int dimensions_len = snprintf(dimensions, sizeof(dimensions),
										  "%d %d\n", col, line);
			fwrite(dimensions, sizeof(char), dimensions_len, output);
		}

		if (type == 3 || type == 6) { // P3 or P6 (RGB)
			for (int i = 0; i < line; i++) {
				for (int j = 0; j < col; j++) {
					unsigned char r = (unsigned char)picture[i][j].r;
					unsigned char g = (unsigned char)picture[i][j].g;
					unsigned char b = (unsigned char)picture[i][j].b;

					fwrite(&r, sizeof(unsigned char), 1, output);
					fwrite(&g, sizeof(unsigned char), 1, output);
					fwrite(&b, sizeof(unsigned char), 1, output);
				}
			}
		} else { // Other types
			for (int i = 0; i < line; i++) {
				for (int j = 0; j < col; j++) {
					unsigned char out = (unsigned char)monochrome[i][j];
					fwrite(&out, sizeof(unsigned char), 1, output);
				}
			}
		}
	}

	fclose(output);
	printf("Saved %s\n", file_name);
}

// get the apply command
int apply_command(const char apply_type[51])
{
	if (!(strcmp(apply_type, "EDGE")))
		return 1;
	if (!(strcmp(apply_type, "SHARPEN")))
		return 2;
	if (!(strcmp(apply_type, "BLUR")))
		return 3;
	if (!(strcmp(apply_type, "GAUSSIAN_BLUR")))
		return 4;
	return 0;
}

// clamp function
int clamp(int x)
{
	if (x < 0)
		return 0;
	if (x > 255)
		return 255;
	return x;
}

// apply edge
void edge_apply(const int col, const int line, const int x1, const int x2,
				const int y1, const int y2, color ***picture)
{
	int aux[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
	color_double **result; // aux matrix
	alloc_double(x2 - x1, y2 - y1, &result);

	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			if (i > 0 && i < line - 1 && j > 0 && j < col - 1) { // interior
				color_double sum = {0.0, 0.0, 0.0};
				for (int ki = -1; ki <= 1; ki++) {
					for (int lj = -1; lj <= 1; lj++) {
						sum.r += (double)((*picture)[i + ki][j + lj].r *
								aux[ki + 1][lj + 1]);
						sum.g += (double)((*picture)[i + ki][j + lj].g *
								aux[ki + 1][lj + 1]);
						sum.b += (double)((*picture)[i + ki][j + lj].b *
								aux[ki + 1][lj + 1]);
					}
				}
				result[k][l] = sum;
			} else { // on edges
				result[k][l].r = (double)(*picture)[i][j].r;
				result[k][l].g = (double)(*picture)[i][j].g;
				result[k][l].b = (double)(*picture)[i][j].b;
			}
		}
	}
	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			(*picture)[i][j].r = clamp(round(result[k][l].r));
			(*picture)[i][j].g = clamp(round(result[k][l].g));
			(*picture)[i][j].b = clamp(round(result[k][l].b));
		}
	}
	free_double(y2 - y1, result);
}

// apply sharpen
void sharpen_apply(const int col, const int line, const int x1,
				   const int x2, const int y1, const int y2, color ***picture)
{
	int aux[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
	color_double **result;
	alloc_double(x2 - x1, y2 - y1, &result);

	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			if (i > 0 && i < line - 1 && j > 0 && j < col - 1) {
				color_double sum = {0.0, 0.0, 0.0};
				for (int ki = -1; ki <= 1; ki++) {
					for (int lj = -1; lj <= 1; lj++) {
						sum.r += (double)((*picture)[i + ki][j + lj].r *
								aux[ki + 1][lj + 1]);
						sum.g += (double)((*picture)[i + ki][j + lj].g *
								aux[ki + 1][lj + 1]);
						sum.b += (double)((*picture)[i + ki][j + lj].b *
								aux[ki + 1][lj + 1]);
					}
				}
				result[k][l] = sum;
			} else {
				result[k][l].r = (double)(*picture)[i][j].r;
				result[k][l].g = (double)(*picture)[i][j].g;
				result[k][l].b = (double)(*picture)[i][j].b;
			}
		}
	}
	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			(*picture)[i][j].r = clamp(round(result[k][l].r));
			(*picture)[i][j].g = clamp(round(result[k][l].g));
			(*picture)[i][j].b = clamp(round(result[k][l].b));
		}
	}
	free_double(y2 - y1, result);
}

// apply blur
void blur_apply(const int col, const int line, const int x1, const int x2,
				const int y1, const int y2, color ***picture)
{
	int aux[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	color_double **result;
	alloc_double(x2 - x1, y2 - y1, &result);

	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			if (i > 0 && i < line - 1 && j > 0 && j < col - 1) {
				color_double sum = {0.0, 0.0, 0.0};
				for (int ki = -1; ki <= 1; ki++) {
					for (int lj = -1; lj <= 1; lj++) {
						sum.r += ((double)((*picture)[i + ki][j + lj].r *
								aux[ki + 1][lj + 1])) / 9;
						sum.g += ((double)((*picture)[i + ki][j + lj].g *
								aux[ki + 1][lj + 1])) / 9;
						sum.b += ((double)((*picture)[i + ki][j + lj].b *
								aux[ki + 1][lj + 1])) / 9;
					}
				}
				result[k][l] = sum;
			} else {
				result[k][l].r = (double)(*picture)[i][j].r;
				result[k][l].g = (double)(*picture)[i][j].g;
				result[k][l].b = (double)(*picture)[i][j].b;
			}
		}
	}
	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			(*picture)[i][j].r = clamp(round(result[k][l].r));
			(*picture)[i][j].g = clamp(round(result[k][l].g));
			(*picture)[i][j].b = clamp(round(result[k][l].b));
		}
	}
	free_double(y2 - y1, result);
}

// apply gaussian_blur
void gaussianblur_apply(const int col, const int line, const int x1,
						const int x2, const int y1, const int y2,
						color ***picture)
{
	int aux[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
	color_double **result;
	alloc_double(x2 - x1, y2 - y1, &result);

	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			if (i > 0 && i < line - 1 && j > 0 && j < col - 1) {
				color_double sum = {0.0, 0.0, 0.0};
				for (int ki = -1; ki <= 1; ki++) {
					for (int lj = -1; lj <= 1; lj++) {
						sum.r += ((double)((*picture)[i + ki][j + lj].r *
								aux[ki + 1][lj + 1])) / 16;
						sum.g += ((double)((*picture)[i + ki][j + lj].g *
								aux[ki + 1][lj + 1])) / 16;
						sum.b += ((double)((*picture)[i + ki][j + lj].b *
								aux[ki + 1][lj + 1])) / 16;
					}
				}
				result[k][l] = sum;
			} else {
				result[k][l].r = (double)(*picture)[i][j].r;
				result[k][l].g = (double)(*picture)[i][j].g;
				result[k][l].b = (double)(*picture)[i][j].b;
			}
		}
	}
	for (int i = y1, k = 0; i < y2; i++, k++) {
		for (int j = x1, l = 0; j < x2; j++, l++) {
			(*picture)[i][j].r = clamp(round(result[k][l].r));
			(*picture)[i][j].g = clamp(round(result[k][l].g));
			(*picture)[i][j].b = clamp(round(result[k][l].b));
		}
	}
	free_double(y2 - y1, result);
}

// apply main function
int apply(const bool loaded, const int type, const int col, const int line,
		  const int x1, const int x2, const int y1, const int y2,
		  color ***picture)
{
	if (!loaded) {
		printf("No image loaded\n");
		get_line();
		return 0;
	}
	char buffer, apply_type[51];
	scanf("%c", &buffer);
	if (buffer == '\n') {
		printf("Invalid command\n");
		return 0;
	}
	scanf("%s", apply_type);
	if (!(type == 3 || type == 6)) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	if (!apply_command(apply_type)) {
		printf("APPLY parameter invalid\n");
		return 0;
	}
	switch (apply_command(apply_type)) {
	case 1:
		edge_apply(col, line, x1, x2, y1, y2, picture);
		printf("APPLY %s done\n", apply_type);
		return 1;
	case 2:
		sharpen_apply(col, line, x1, x2, y1, y2, picture);
		printf("APPLY %s done\n", apply_type);
		return 1;
	case 3:
		blur_apply(col, line, x1, x2, y1, y2, picture);
		printf("APPLY %s done\n", apply_type);
		return 1;
	case 4:
		gaussianblur_apply(col, line, x1, x2, y1, y2, picture);
		printf("APPLY %s done\n", apply_type);
		return 1;
	}
	return 0;
}

//histogram function
void histogram(const int loaded, const int type, const int col,
			   const int line, int **picture)
{
	if (!loaded) {
		printf("No image loaded\n");
		get_line();
		return;
	}
	char arg1[5], arg2[5], buffer;
	int bins, stars;
	scanf("%c", &buffer);
	if (buffer == '\n') {
		printf("Invalid command\n");
		return;
	}
	scanf("%s", arg1);
	if (arg1[0] >= '0' && arg1[0] <= '9') {
		stars = atoi(arg1);
	} else {
		printf("Invalid command\n");
		get_line();
		return;
	}
	scanf("%c", &buffer);
	if (buffer == '\n') {
		printf("Invalid command\n");
		return;
	}
	scanf("%s", arg2);
	if (arg2[0] >= '0' && arg2[0] <= '9') {
		bins = atoi(arg2);
	} else {
		printf("Invalid command\n");
		get_line();
		return;
	}
	scanf("%c", &buffer);
	if (buffer != '\n') {
		printf("Invalid command\n");
		get_line();
		return;
	}
	if (type == 3 || type == 6) {
		printf("Black and white image needed\n");
		return;
	}
	int *histogram = calloc(bins, sizeof(int));
	if (!histogram) {
		printf("Memory allocation failed\n");
		return;
	}
	double bin_size = 256.0 / bins;
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < col; j++) {
			int pixel_value = picture[i][j];
			int bin_index = (int)floor((double)pixel_value / (double)bin_size);

			histogram[bin_index]++;
		}
	}
	double frequency = 0.0;
	for (int i = 0; i < bins; i++) {
		if (histogram[i] > frequency) {
			frequency = (double)histogram[i];
		}
	}
	int max_frequency = (int)floor(frequency);
	for (int i = 0; i < bins; i++) {
		double ratio = ((double)histogram[i] /
					  (double)max_frequency) * (double)stars;
		int num_stars = (int)floor(ratio);
		printf("%d\t|\t", num_stars);
		for (int j = 0; j < num_stars; j++) {
			printf("*");
		}
		printf("\n");
	}
	free(histogram);
}

// equalize function
int equalize(const int loaded, const int type, const int col,
			 const int line, int ***picture)
{
	if (!loaded) {
		printf("No image loaded\n");
		get_line();
		return 0;
	}

	if (type == 3 || type == 6) {
		printf("Black and white image needed\n");
		get_line();
		return 0;
	}

	int area = col * line;

	int *histogram = (int *)calloc(256, sizeof(int));
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < col; j++) {
			int pixel_value = (*picture)[i][j];
			histogram[pixel_value]++;
		}
	}

	double *cumulative_histogram = (double *)calloc(256, sizeof(double));
	cumulative_histogram[0] = histogram[0];
	for (int i = 1; i < 256; i++) {
		cumulative_histogram[i] = cumulative_histogram[i - 1] + histogram[i];
	}

	for (int i = 0; i < line; i++) {
		for (int j = 0; j < col; j++) {
			int pixel_value = (*picture)[i][j];
			double normalized_value = 255.0 *
									 cumulative_histogram[pixel_value] / area;
			int new_pixel_value = clamp((int)round(normalized_value));

			(*picture)[i][j] = new_pixel_value;
		}
	}

	free(histogram);
	free(cumulative_histogram);

	printf("Equalize done\n");
	return 1;
}
