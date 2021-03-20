#include "ast/ast_declaration.hpp"

// Constructors
/* Legacy code, refer back if needed
// For scaling, not necessary atm (probably lmao)
// Basically, we're gonna always have a declaration as a type at branches[0] and then a list of declarations at branches[1]
Declaration::Declaration(NodePtr type, NodeListPtr declarations)
{
  branches.push_back(type);
  branches.push_back(declarations);
}

Declaration::Declaration(NodePtr type, std::vector<NodePtr> declarations)
  : Declaration(type, new NodeList(declarations))
{}

// Case where it's just 1 declaration (probs the one we'll usually use)
Declaration::Declaration(NodePtr type, NodePtr declaration)
  : Declaration(type, std::vector<NodePtr>{declaration})
{}

// idk if we ever have a declaration with just a type, this is here just in case ig
Declaration::Declaration(NodePtr type)
  : Declaration(type, std::vector<NodePtr>{})
{}
*/
Declaration::Declaration(NodePtr type, NodePtr id)
{
  branches.push_back(type);
  branches.push_back(id);
}

Declaration::Declaration(NodePtr type)
  : Declaration(type, new Identifier("<NULL>"))
{}

// Destructor
Declaration::~Declaration()
{
  delete branches[0];
  delete branches[1];
}

// Where things in should be
NodePtr Declaration::getType() const
{
  return branches[0];
}

NodePtr Declaration::getDeclarations() const // Actually returns a NodeListPtr (but it's stored as a NodePtr)
{
  return branches[1];
}

int Declaration::getSize() const
{
  return branches[0]->getSize();
}

std::string Declaration::getId() const
{
  return branches[1]->getId();
}

// Visualising
void Declaration::PrettyPrint(std::ostream &dst, std::string indent) const
{
  dst << indent << "Declaration [" << std::endl;
  dst << indent+"  " << "Type: ";
  branches[0]->PrettyPrint(dst, indent+"  ");
  branches[1]->PrettyPrint(dst, indent+"  ");
  dst << indent << "]" << std::endl;
}

void Declaration::generateMIPS(std::ostream &dst, Context &context, int destReg) const
{
  // Check the type of branches1 to see if it's a variable / function declaration
  int size = branches[0]->getSize();
  if(branches[1]->isFunction()){
    // Deals with function declaration
    branches[1]->generateMIPS(dst, context, destReg);
  }
  else{
    // Deals with variable declaration
    std::string id = branches[1]->getId();
    context.stack.back().offset += size; // Increments stack offset to have space for variable, will actually store it when its value is assigned
    dst << "addiu $29,$29,-" << size << std::endl; // Increments stack pointer

    // If the variable is being initialised (will be similar to assignment operator)
    if(branches[1]->isInit()){
      // Ensures a free register is being used
      if(context.regFile.usedRegs[destReg]){
        destReg = context.regFile.allocate();
      }
      // If no registers are free
      if(destReg == -1){
        for(auto it = context.stack.back().varBindings.begin(); it != context.stack.back().varBindings.end(); it++){
          if (it->second.reg != -1){
            context.regFile.freeReg(it->second.reg); // This variable can still be accessed directly from memory
            destReg = it->second.reg;
            it->second.reg = -1; // Inidicate this variable is no longer available in register
          }
        }
      }
      branches[1]->generateMIPS(dst, context, destReg); // Evaluates initializer into allocated register
      context.stack.back().varBindings[id] = {size, context.stack.back().offset, destReg}; // stores the space allocated
      context.regFile.useReg(destReg); // Indicates register is being used
      dst << "sw $" << destReg << ",0($29)" << std::endl; // Stores variable in memory allocated
    }
    // Variable is not initialised
    else{
      context.stack.back().varBindings[id] = {size, context.stack.back().offset, -1}; // stores the space allocated (currently not available in a register)
    }

  }
}
