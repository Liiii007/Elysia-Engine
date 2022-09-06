ComponentName = input("Component Name:")

CppFileName = ComponentName + ".cpp"
HeaderName = ComponentName + ".h"

Th = """#pragma once
#include <string>
#include <Interface/IComponent.h>
#include <Interface/ComponentBase.h>

class Entity;
//Define "class X;" here

class {0} : public ComponentBase {{

public:
    
    {0}(Entity* entity);
    static std::string componentName;
    std::string name;
	
    //ReturnType GetValue();
    //{0}* SetValue(value);//Return the pointer for Chain calls
    Entity& ReturnParentEntity();//Same as above

    //Some Data Member
}};
""".format(ComponentName)

TCpp = """//Note:For dependent component, include X.h here, define "class X;" in T.h
#include "{0}.h"
#include <World/Entity.h>
std::string {0}::componentName = "{0}";
{0}::{0}(Entity* entity) : ComponentBase(entity) {{}}

Entity& {0}::ReturnParentEntity() {{
	return *parentEntity;//Define in ComponentBase.h
}}
""".format(ComponentName)

try:
    with open(CppFileName, "x") as f:
        f.write(TCpp)
        with open("FullComponentClass.h", "a") as FullComponentClass:
            FullComponentClass.write("\nclass {};".format(ComponentName))

        with open("FullComponentHeader.h", "a") as FullComponentClass:
            FullComponentClass.write("\n#include <Components/{}.h>".format(ComponentName))

    with open(HeaderName, "x") as f:
        f.write(Th)
except Exception:
    print("Already Exist!")
else:
    print("Component {} Generated!".format(ComponentName))
