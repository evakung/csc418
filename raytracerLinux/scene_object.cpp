/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"
#include <stdio.h>

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	Ray3D ray_model;
    ray_model.origin = worldToModel * ray.origin;
    ray_model.dir = worldToModel * ray.dir;

    //calculates the value t where the unit square intersects with the ray
	float t;
	t = -(ray_model.origin[2]/ray_model.dir[2]);
    if (t <= 0) {
        return false;
    }
    
    // checking both the x and the y values by calculating them both
    float X = ray_model.origin[0] + t*ray_model.dir[0];
    float Y = ray_model.origin[1] + t*ray_model.dir[1];
    Point3D cross_point(X, Y, 0.0); // create the intersection point variable
    Vector3D norm(0.0,0.0,1.0);
    
    //update value(s) of the ray if it intersects 
    if ( (X <= 0.5 && X >= -0.5) && (Y <= 0.5 && Y >= -0.5) ) {
        if (ray.intersection.none || t < ray.intersection.t_value) {
            ray.intersection.t_value = t;
            ray.intersection.point = modelToWorld*cross_point;
            ray.intersection.normal = worldToModel.transpose()*norm;
            ray.intersection.normal.normalize();
            ray.intersection.none = false;
            return true;
        }
    }
	return false;
}


bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	//coordinates of the object space we are working with 
	
	Point3D ray_model;
	ray_model = worldToModel * ray.origin;
	
	Vector3D ray_direction;
	ray_direction = worldToModel * ray.dir;

	Point3D center_of_sphere(0, 0, 0); //the point of the center of the sphere
	Vector3D distance_of_sphere = ray_model - center_of_sphere; //calculate the distance


    /* derive the points through the use of the quadratic formula, 
	** also initialize and assign values of all relevant variables 
    */
    double a = ray_direction.dot(ray_direction);
	double b = distance_of_sphere.dot(ray_direction);
	double c = distance_of_sphere.dot(distance_of_sphere) - 1;
	double d = b * b - a * c;
	double t = -1;

	if (d == 0) {
		// there is one intersection point only
		t = -b / a;

	} else if (d > 0) {

		// there are two intersection points that exist
		double t_0 = (-b + sqrt(d)) / a;
		double t_1 = (-b - sqrt(d)) / a;
		
		//conditions for checking for negative values 
		if ( t_0 < 0 && t_1 < 0 ){
		    return false;
		
		}else if ( t_0 < 0 ){
		    t = t_1; //set t
		
		}else if ( t_1 < 0){
		    t = t_0;
		
		}else{
		    t = fmin(t_1,t_0);
		}

	}else if (d < 0) {
		return false; //no existing intersection found
	}

	if (t <= 0) { // nonexisting cross point in our view space
		return false;
	}

	if (ray.intersection.none || t < ray.intersection.t_value) {

		Point3D intersection_point = ray_model + t * ray_direction;
		Vector3D normal_vector = 2 * (intersection_point - center_of_sphere);
		normal_vector.normalize();

		// update all of the values of ray  
		ray.intersection.t_value = t;
		ray.intersection.point = modelToWorld * intersection_point;
		ray.intersection.normal = transNorm(worldToModel, normal_vector);
		ray.intersection.none = false;

		return true;
	}
	return false;
}

