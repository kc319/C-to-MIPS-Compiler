#include "ast/ast_return.hpp"

// Constructors
Return::Return(NodePtr val)
{
  branches.push_back(val);
}

Return::Return() // For void funcitons (hopefully this works), returns 0 (i.e. success)
  : Return(new Integer())
{}

// Destructor
Return::~Return()
{
  delete branches[0];
}

// Visualising
virtual void Return::PrettyPrint(std::ostream &dst, std::string indent) const override
{
  dst << indent << "Return: [" << std::endl;
  branches[0]->PrettyPrint(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}
