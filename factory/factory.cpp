﻿#define _USE_MATH_DEFINES
#include<Windows.h>
#include<iostream>
using namespace std;

//#define MIN_SIZE		50
//#define MAX_SIZE		800
//#define MIN_LINE_WIDTH	1
//#define MAX_LINE_WIDTH	25
//
//#define MAX_HORIZONTAL_RESOLUTION	800
//#define MAX_VERTICAL_RESOLUTION		600

namespace MyGeometry
{
	enum Color
	{
		RED = 0x000000FF,
		GREEN = 0x0000FF00,
		BLUE = 0x00FF0000,
		GREY = 0x00AAAAAA,
		YELLOW = 0x0000FFFF
	};

#define SHAPE_TAKE_PARAMETRS unsigned int x, unsigned int y, unsigned int line_width = 5, Color color = Color::GREY
#define SHAPE_GIVE_PARAMETRS x, y, line_width, color
	class Shape
	{
	protected:
		static const int MIN_SIZE = 50;
		static const int MAX_SIZE = 800;
		static const int MIN_LINE_WIDTH = 1;
		static const int MAX_LINE_WIDTH = 25;
		static const int MAX_HORIZONTAL_RESOLUTION = 800;
		static const int MAX_VERTICAL_RESOLUTION = 600;

		unsigned int x;
		unsigned int y;
		unsigned int line_width;
		Color color;
	public:

		unsigned int get_x()const
		{
			return x;
		}
		unsigned int get_y()const
		{
			return y;
		}
		unsigned int get_line_width()const
		{
			return line_width;
		}
		unsigned int set_size(unsigned int size)
		{
			return
				size < MIN_SIZE ? MIN_SIZE :
				size > MAX_SIZE ? MAX_SIZE :
				size;
		}
		void set_x(unsigned int x)
		{
			this->x = x < MAX_HORIZONTAL_RESOLUTION ? x : MAX_HORIZONTAL_RESOLUTION;
		}
		void set_y(unsigned int y)
		{
			this->y = y < MAX_VERTICAL_RESOLUTION ? y : MAX_VERTICAL_RESOLUTION;
		}
		void set_line_width(unsigned int line_width)
		{
			if (line_width < MIN_LINE_WIDTH)line_width = MIN_LINE_WIDTH;
			if (line_width > MAX_LINE_WIDTH)line_width = MAX_LINE_WIDTH;
			this->line_width = line_width;
		}

		//				Constructors:
		Shape(SHAPE_TAKE_PARAMETRS) :color(color)
		{
			set_x(x);
			set_y(y);
			set_line_width(line_width);
		}
		virtual ~Shape() {}

		//				Methods:
		virtual double get_area()const = 0;
		virtual double get_perimeter()const = 0;
		virtual void draw()const = 0;

		virtual void info()const
		{
			cout << "Площадь фигуры: " << get_area() << endl;
			cout << "Периметр фигуры: " << get_perimeter() << endl;
			draw();
		}
	};

	class Rectangle :public Shape
	{
		double width;
		double height;
	public:
		double get_width()const
		{
			return width;
		}
		double get_height()const
		{
			return height;
		}
		void set_width(double width)
		{
			this->width = set_size(width);
		}
		void set_height(double height)
		{
			this->height = set_size(height);
		}

		//				Constructors:
		Rectangle(double width, double height, unsigned int x, unsigned int y, unsigned int line_width, Color color) :
			Shape(SHAPE_GIVE_PARAMETRS)
		{
			set_width(width);
			set_height(height);
		}
		~Rectangle() {}

		//				Methods:
		double get_area()const override
		{
			return width * height;
		}
		double get_perimeter()const override
		{
			return (width + height) * 2;
		}
		void draw()const override
		{
			/*for (int i = 0; i < heigth; i++)
			{
				for (int j = 0; j < width; j++)
				{
					cout << "* ";
				}
				cout << endl;
			}*/

			//HWND - Handler to Window (Дескриптор окна, нужен для того чтобы обращаться в ону)
			HWND hwnd = GetConsoleWindow();	//Получаем дескриптор окна консоли.
			//HWND hwnd = GetDesktopWindow();	//Получаем дескриптор окна консоли.

			//Для того чтобы рисовать, нужен контекст устройства (Device Context), он есть каждого окна
			HDC hdc = GetDC(hwnd);
			//hdc - это то, на чем мы будем рисовать.

			//Теперь нужно то, чем мы будем рисовать:

			HPEN hPen = CreatePen(PS_SOLID, line_width, color);	//Карандаш рисует контур фигуры
			HBRUSH hBrush = CreateSolidBrush(color);	//Кисть заливает фигуру

			//Теперь нужно выбрать чем и на чем будем рисовать:
			SelectObject(hdc, hPen);
			SelectObject(hdc, hBrush);

			::Rectangle(hdc, x, y, x + width, y + height);

			//Удаляем все созданные объекты для того чтобы освободить ресурсы:

			DeleteObject(hPen);
			DeleteObject(hBrush);

			ReleaseDC(hwnd, hdc);
		}
		void info()const override
		{
			cout << typeid(*this).name() << endl;
			cout << "Ширина: " << width << endl;
			cout << "Высота: " << height << endl;
			Shape::info();
		}
	};
	class Square :public Rectangle
	{
	public:
		Square(double side, SHAPE_TAKE_PARAMETRS) :Rectangle(side, side, SHAPE_GIVE_PARAMETRS) {}
		~Square() {}
	};

	class Circle : public Shape
	{
		double radius;
	public:
		double get_radius()const {
			return radius;
		}
		double get_diameter()const {
			return 2 * radius;
		}
		void set_radius(double radius)
		{
			this->radius = set_size(radius);
		}
		double get_area()const {
			return M_PI * radius * radius;
		}
		double get_perimeter()const {
			return 2 * M_PI * radius;
		}
		Circle(double radius, SHAPE_TAKE_PARAMETRS) :Shape(SHAPE_GIVE_PARAMETRS) {
			set_radius(radius);
		}
		~Circle() {}

		void draw()const override {
			HWND hwnd = GetConsoleWindow();

			HDC hdc = GetDC(hwnd);

			HPEN hPen = CreatePen(PS_SOLID, line_width, color);
			HBRUSH hBrush = CreateSolidBrush(color);

			SelectObject(hdc, hPen);
			SelectObject(hdc, hBrush);

			::Ellipse(hdc, x, y, x + get_diameter(), y + get_diameter());

			DeleteObject(hPen);
			DeleteObject(hBrush);
			ReleaseDC(hwnd, hdc);

		}
		void info()const override
		{
			cout << typeid(*this).name() << endl;
			cout << "Радиус: " << get_radius() << endl;
			cout << "Диаметр: " << get_diameter() << endl;
			Shape::info();
		}
	};

}

void main()
{
	setlocale(LC_ALL, "");
	MyGeometry::Rectangle rect(100, 50, 100, 200, 8, MyGeometry::Color::RED);
	/*cout << "Ширина прямоугольника: " << rect.get_width() << endl;
	cout << "Высота прямоугольника: " << rect.get_height() << endl;
	cout << "Площадь прямоугольника: " << rect.get_area() << endl;
	cout << "Периметр прямоугольника: " << rect.get_perimeter() << endl;
	rect.draw();*/
	rect.info();

	MyGeometry::Square square(44, 250, 200, 5, MyGeometry::Color::GREY);
	square.info();

	MyGeometry::Circle cyrcle(75, 750, 100, 5, MyGeometry::Color::YELLOW);
	cyrcle.info();
}