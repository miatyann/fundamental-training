#include <iostream>
#include <memory>

//#define SIMPLE
#define ROBUST

class shape {
public:
    shape() {
        id_ = total_++;
        std::cout << "shape constructor...\n";
    }

    ~shape() {
        std::cout << "shape destructor...\n";
    }

    virtual void draw() = 0;

protected:
    int id_;
    static int total_;
};

int shape::total_ = 0;

class circle: public shape {
public:
    void draw() {
        std::cout << "circle " << id_ << ": draw\n";
    }
};

class square: public shape {
public:
    void draw() {
        std::cout << "square " << id_ << ": draw\n";
    }

    square() {
        std::cout << "square constructor...\n";
    }

    ~square() {
        std::cout << "square destructor...\n";
    }
};

class ellipse: public shape {
public:
    void draw() {
        std::cout << "ellipse " << id_ << ": draw\n";
    }

    ellipse() {
        std::cout << "ellipse constructor...\n";
    }

    ~ellipse() {
        std::cout << "ellipse destructor...\n";
    }
};

class rectangle: public shape {
public:
    void draw() {
        std::cout << "rectangle " << id_ << ": draw\n";
    }

    rectangle() {
        std::cout << "rectangle constructor...\n";
    }

    ~rectangle() {
        std::cout << "rectangle destructor...\n";
    }
};

using shape_shared_ptr = std::shared_ptr<shape>;

class factory {
public:
    virtual shape_shared_ptr create_curved_instance() = 0;
    virtual shape_shared_ptr create_straight_instance() = 0;
    factory() {
        std::cout << "factory constructor...\n";
    }

    ~factory() {
        std::cout << "factory destructor...\n";
    }
};

class simple_shape_factory: public factory {
public:
    shape_shared_ptr create_curved_instance() {
        return std::make_shared<circle>();
    }

    shape_shared_ptr create_straight_instance() {
        return std::make_shared<square>();
    }

    simple_shape_factory() {
        std::cout << "simple_shape_factory constructor...\n";
    }

    ~simple_shape_factory() {
        std::cout << "simple_shape_factory destructor...\n";
    }
};

class robust_shape_factory: public factory {
public:
    shape_shared_ptr create_curved_instance() {
        return std::make_shared<ellipse>();
    }

    shape_shared_ptr create_straight_instance() {
        return std::make_shared<rectangle>();
    }

    robust_shape_factory() {
        std::cout << "robust_shape_factory constructor...\n";
    }

    ~robust_shape_factory() {
        std::cout << "robust_shape_factory destructor...\n";
    }
};


int main(void)
{
#if defined(SIMPLE)
    auto fac = std::make_shared<simple_shape_factory>();
#elif defined(ROBUST)
    auto fac = std::make_shared<robust_shape_factory>();
#endif

    shape_shared_ptr shapes[3];

    shapes[0] = fac->create_curved_instance();
    shapes[1] = fac->create_straight_instance();
    shapes[2] = fac->create_curved_instance();

    for (auto s: shapes) {
        s->draw();
    }

    return 0;
}