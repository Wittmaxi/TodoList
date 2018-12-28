#include <webcpp/webcpp.hpp>
#include <wolfrini/wolfrini.hpp>
#include <fstream>
#include <sstream>
using namespace WCP;



struct task {
    task (std::string t) {
        type = std::string (t.begin(), std::find (t.begin(), t.end(), ':'));
        name = std::string (++std::find (t.begin(), t.end(), ':'), t.end());
    }
    std::string type;
    std::string name;
};

std::vector<task> tasks;
bool reset = false;
std::fstream tasks_f("todo.txt"); //type: task


void loadTasks () {
    for(std::string line; getline( tasks_f, line );)
        tasks.push_back (task(line));
}


void saveTasksToFile () {
    std::ofstream tasks_of ("todo.txt");
    for (auto i : tasks)
        tasks_of << i.type << ":" << i.name << "\n";
}

void testResponse() {
    if (ENV::POST("task") != "") {
        tasks.push_back(ENV::POST("cathegory") + ":" + ENV::POST("task"));
    } else if (ENV::POST("deletename") != "") { 
        for (auto i = tasks.begin(); i != tasks.end(); i++)
            if (i->name == ENV::POST("deletename")) {
                tasks.erase (i);
                break;
            }
    }
    else
        return;
    reset = true;
    saveTasksToFile(); 
}

Container generateForm (const std::string& Type) {
    return Container {StyleAttribute {"margin-bottom: 2em;"}, ClassAttribute{"col-sm-12 col-sm-6"},
        H4 {
            ConvenientText {Type}
        },
        Table {ClassAttribute{"table table-dark table-striped"},
            Function {
                [=]() {
                for (auto i : tasks) 
                    if (i.type == Type)
                        wcpout << Row { 
                            Cell { 
                                ConvenientText{i.name}
                            }, 
                            Cell {
                                Form {FormMethod{"POST"},
                                    Input {InputType{"hidden"}, InputValue{i.name}, Attribute{"name=\"deletename\""}},
                                    Input{ClassAttribute {"form-control text-grey"}, InputType{"submit"}, InputValue{"Delete Task"}}
                                }
                            }
                        };
                }
            }
        },
        Form {FormMethod{"POST"}, ClassAttribute{"form-inline"},
            Input {InputType{"hidden"}, InputValue{Type}, Attribute{"name=\"cathegory\""}},
            Container {ClassAttribute{"col-12 form-row"},
                Input {ClassAttribute {"form-control col-10"}, InputType{"input"}, Attribute{"name=\"task\""}, Attribute{"autocomplete=\"off\""}, Attribute{"placeholder=\"Enter your task!\""}},
                Input {ClassAttribute {"form-control btn btn-secondary col-2"}, InputType{"submit"}}
            }
        }
    };
}

int main()
{
    HTTPHeader myHeader;
    myHeader.print();
    ENV::initializeENV();
    loadTasks();
    testResponse();
    Document project{
        Head {
            Include {"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css", "text/css", "stylesheet"},
            IncludeScript {"https://code.jquery.com/jquery-3.3.1.slim.min.js"},
            IncludeScript {"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js"},
            IncludeScript {"https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js"},
            HttpEquiv ("refresh", reset ? "0" : "300")
        },
        Body{ClassAttribute{"bg-dark text-white"},
            H5 {ClassAttribute{"alert alert-info"},
                ConvenientText ("You currently have " + std::to_string(tasks.size()) + " Tasks open!")
            },
            Break,
            HorizontalLine{},
            Break,
            Container {ClassAttribute ("row"),
                generateForm("Important/Pressing"),
                generateForm("Important/Nonpressing"),
                generateForm("Unimportant/Pressing"),
                generateForm("Unimportant/Nonpressing"),
                generateForm("Other"),
                generateForm("School/Work")
            }
        }
    };
    
    std::cout << project;
}
