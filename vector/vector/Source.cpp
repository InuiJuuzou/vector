#include <iostream>
#include <new>
#include <string>


template<typename T>
class Vector {
private:
	T* container_ = nullptr;
	size_t size_ = 0;
	size_t capasity_ = 0;

	struct Iterator
	{
		T* iter = nullptr;//�������� �� ������

		Iterator(T* obj) :iter(obj)
		{}

		T& operator*() {
			return *iter;
		}
		T* operator+(int i)
		{
			return iter + i;
		}
		T* operator-(int i)
		{
			return std::prev(iter);
		}
		T* operator++()
		{
			iter += 1;
			return iter;
		}
		T* operator++(int)
		{
			T* tmp = iter;
			iter += 1;
			return tmp;
		}
		bool operator!=(const Iterator& rhs)
		{
			return iter != rhs.iter;
		}
		bool operator==(const Iterator& rhs)
		{
			return iter == rhs.iter;
		}

	};

public:
	Vector() = default;
	Vector(size_t size) : capasity_(size)
	{
		try {
			//container_ = new T[size];
			//container_ = reinterpret_cast<T*>(new uint8_t[sizeof(T)*size]);
			container_ = new T[sizeof(T) * size];
		}
		catch (...)
		{
			throw;
		}
	}
	Vector(size_t size, T value) :size_(size), capasity_(size)
	{
		try {
			//����������������� � ������� ������ ������������ �� ������� ������
			//placement new
			container_ = new T[sizeof(T) * size];
			for (size_t i = 0; i < size; ++i)
			{
				new(container_ + i)T(value);
			}
		}
		catch (...)
		{
			throw;
		}
	}
	~Vector()
	{
		//������� ������� ���������� ��� ������� ��������
		for (size_t i = 0; i < capasity_; ++i)
		{
			//container_[i].~T();
			(container_ + 1)->~T();
		}
		delete[] container_;
	}
	//����������� �����������
	Vector(const Vector& v) : Vector(v.capasity_)
	{
		size_ = v.size_;
		for (int i = 0; i < v.size_; ++i)
		{
			new(container_ + i)T(v.container_[i]);
		}
	}
	Vector& operator=(const Vector& v)
	{
		//1. ������� ��� ��� ������ ����� � �������
		//2. �������� ������
		//3.�������� �����
		this->~Vector();
		container_ = new T[sizeof(T) * v.size_];
		for (int i = 0; i < v.size_; ++i)
		{
			new(container_ + i)T(v.container_[i]);
		}
		return *this;
	}
	size_t size() const
	{
		return size_;
	}
	size_t capacity() const
	{
		return capasity_;
	}
	bool empty() const
	{
		return size_ == 0;
	}
	void reserve(size_t n)
	{
		//1. �������� ����� ���� ������ � 2 ���� ������ ��������
		//2.����������� ���������
		//3. ����������� �������� � ������ ������(����� ������������)
		//4. ������������ ������ ������
		if (n <= capasity_) return;
		T* temp = reinterpret_cast<T*>(operator new(sizeof(T) * size_));//��������� ����� ������ ������� ������� ���������� void*
		//copy
		for (int i = 0; i < size_; ++i)
		{
			new(temp + i)T(container_[i]);
		}

		//*this->~Vector();
		for (size_t i = 0; i < capasity_; ++i)
		{
			(container_ + 1)->~T();
		}
		delete[] container_;

		container_ = new T[sizeof(T) * n];

		for (int i = 0; i < size_; ++i)
		{
			new(container_ + i)T(temp[i]);
		}

		capasity_ = n;
	}
	void resize(size_t n, T value)//TODO value ������ ��������� �������� �� ���������
	{
		//���� n ������ �������� ������� ����������, ������ �������� � ����� ���������� ������������.
		if (n == size_) return;
		if (n < size_)
		{
			for (int i = n; i < size_; ++i)
			{
				(container_ + i)->~T();
			}
			size_ = n;
			return;
		}
		if (n > size_ && n < capasity_)
		{
			for (int i = size_; i < n; ++i)
			{
				new(container_ + i)T(value);
			}
			size_ = n;
			return;
		}
		reserve(n);
		for (int i = size_; i < n; ++i)
		{
			new(container_ + i)T(value);
		}
		size_ = n;
	}
	void push_back(T elem)
	{
		//���� ���� ��� ���������� ���������� ����� �������
		if (size_ == capasity_)
		{
			reserve(capasity_ * 2);
		}

		new(container_ + size_)T(elem);
		++size_;
	}
	T& operator[](int idx)
	{
		return container_[idx];
	}
	const T& operator[](int idx) const
	{
		return container_[idx];
	}
	T& at(int idx)
	{
		if (idx <0 || idx>size_)
		{
			throw std::out_of_range;
		}
		return container_[idx];
	}
	const T& at(int idx) const
	{
		if (idx <0 || idx>size_)
		{
			throw std::out_of_range;
		}
		return container_[idx];
	}
	Iterator begin()
	{
		return container_;
	}
	Iterator end()
	{
		return container_ + size_;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector& vector);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& vector)
{
	for (int i = 0; i < vector.size_; ++i)
	{
		out << vector.container_[i] << " ";
	}
	return out;
}

int main()
{
	Vector<std::string> v(4);
	std::cout << v.capacity() << " " << v.size() << " " << v.empty() << std::endl;
	v.push_back("a");
	v.push_back("b");
	v.push_back("dddd");
	v.push_back("err");
	std::cout << v.capacity() << " " << v.size() << " " << v.empty() << std::endl;
	//v.resize(2,"s");
	for (int i = 0; i < v.size(); ++i)
	{
		std::cout << v[i] << " - ";
	}
	std::cout << std::endl;
	std::cout << v.capacity() << " " << v.size() << " " << v.empty() << std::endl;
	v.push_back("add");
	for (int i = 0; i < v.size(); ++i)
	{
		std::cout << v[i] << " - ";
	}
	std::cout << std::endl;
	std::cout << v.capacity() << " " << v.size() << " " << v.empty() << std::endl;
	std::cout << *(v.begin() + 3) << '\n';
	for (auto i = v.begin(); i != v.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
	v.resize(8,"t");
	std::cout << v.capacity() << " " << v.size() << " " << v.empty() << std::endl;
	for (auto i = v.begin(); i != v.end(); ++i)
	{
		std::cout << *i << " ";
	}
}