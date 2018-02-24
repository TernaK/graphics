#include <graphics/drawable.h>

using namespace std;
using namespace graphics;

void Drawable::add_child(std::shared_ptr<Drawable> child) {
  children.push_back(child);
}

void Drawable::remove_child(std::shared_ptr<Drawable> child) {
  children.remove_if([&child](const shared_ptr<Drawable>& x) -> bool {
    return child == x;
  });
}

std::vector<std::shared_ptr<Drawable>> Drawable::get_children_with_name(std::string name) {
  vector<shared_ptr<Drawable>> matches;
  for_each(children.begin(), children.end(),
           [&name, &matches](const shared_ptr<Drawable>& x) {
             if(x->name == name)
               matches.push_back(x);
           });
  return matches;
}
