#include <iostream>
#include <memory>
#include <cstdio>
#include <cstring>

enum persistence_type {
    File,
    Queue,
    Pathway
};

struct persistence_attribute {
    persistence_type type;
    char value[30];
};

class distr_work_package {
public:
    distr_work_package(const char *type) 
    {
        sprintf(desc_, "Distributed Work Package for: %s", type);
    }

    void set_file(const char *f, char *v)
    {
        sprintf(temp_, "\n File(%s): %s", f, v);
        strcat(desc_, temp_);
    }
    
    void set_queue(const char *q, char *v)
    {
        sprintf(temp_, "\n Queue(%s): %s", q, v);
        strcat(desc_, temp_);
    }

    void set_pathway(const char *p, char *v)
    {
        sprintf(temp_, "\n Pathway(%s): %s", p, v);
        strcat(desc_, temp_);
    }

    const char *get_state()
    {
        return desc_;
    }

private:
    char desc_[200], temp_[200];
};

class builder {
public:
    virtual void configure_file(char *) = 0;
    virtual void configure_queue(char *) = 0;
    virtual void configure_pathway(char *) = 0;
    std::shared_ptr<distr_work_package> get_result() 
    {
        return result_;
    }

protected:
    std::shared_ptr<distr_work_package> result_;
};

class unix_builder: public builder {
public:
    unix_builder()
    {
        result_ = std::make_shared<distr_work_package>("Unix");
    }

    void configure_file(char *name) override
    {
        result_->set_file("flat_file", name);
    }

    void configure_queue(char *queue) override
    {
        result_->set_queue("fifo", queue);
    }

    void configure_pathway(char *type) override
    {
        result_->set_pathway("thread", type);
    }
};

class vms_builder: public builder {
public:
    vms_builder()
    {
        result_ = std::make_shared<distr_work_package>("Vms");
    }

    void configure_file(char *name) override
    {
        result_->set_file("ISAM", name);
    }

    void configure_queue(char *queue) override
    {
        result_->set_queue("priority", queue);
    }

    void configure_pathway(char *type) override
    {
        result_->set_pathway("LWP", type);
    }
};

class Reader {
public:
    void set_builder(std::shared_ptr<builder> b)
    {
        builder_ = b;
    }

    void construct(persistence_attribute[], int);

private:
    std::shared_ptr<builder> builder_;
};

void Reader::construct(persistence_attribute list[], int num)
{
    for (int i = 0; i < num; ++i) {
        if (File == list[i].type) {
            builder_->configure_file(list[i].value);
        } else if (Queue == list[i].type) {
            builder_->configure_queue(list[i].value);
        } else if (Pathway == list[i].type) {
            builder_->configure_pathway(list[i].value);
        }
    }
}

const int NUM_ENTRIES = 6;
persistence_attribute input[NUM_ENTRIES] = {
    {File, "state.dat"},
    {File, "config.sys"},
    {Queue, "compute"},
    {Queue, "log"},
    {Pathway, "authentication"},
    {Pathway, "error processing"}
};

int main(void)
{
    Reader reader;

    auto unixb = std::make_shared<unix_builder>();

    reader.set_builder(unixb);
    reader.construct(input, NUM_ENTRIES);
    std::cout << unixb->get_result()->get_state() << std::endl;

    auto vmsb = std::make_shared<vms_builder>();

    reader.set_builder(vmsb);
    reader.construct(input, NUM_ENTRIES);
    std::cout << vmsb->get_result()->get_state() << std::endl;

    return 0;
}