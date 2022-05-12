/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

#ifndef VECTOR_H
#define VECTOR_H

struct vector2d {
	float x;
	float y;
};

void add_vector(struct vector2d* a, struct vector2d* b);

struct vector2d add_vector_new(struct vector2d* a, struct vector2d* b);

void multiply_vector(struct vector2d* v, float n);

void divide_vector(struct vector2d* v, float n);

void print_vector(struct vector2d* a);

void rotate_vector(struct vector2d* v, float degrees);

float magnitude_vector(struct vector2d* v);

void normalise_vector(struct vector2d* v);

void limit_vector(struct vector2d* v, float limit);

#endif //VECTOR_H
