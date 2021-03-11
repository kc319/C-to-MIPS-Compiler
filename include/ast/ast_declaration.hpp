#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_node.hpp"
#include "ast_nodeList.hpp"

// Represents declarations as type (i.e. int) + what the declaration actually is, which could be a bunch of stuff

class Declaration
  : public Node
{
public:
  // Constructors
  // For scaling, not necessary atm (probably lmao)
  // Basically, we're gonna always have a declaration as a type at branches[0] and then a list of declarations at branches[1]
  Declaration(NodePtr type, NodeListPtr declarations);
  Declaration(NodePtr type, std::vector<NodePtr> declarations);
  // Case where it's just 1 declaration (probs the one we'll usually use)
  Declaration(NodePtr type, NodePtr declaration);
  // idk if we ever have a declaration with just a type, this is here just in case ig
  Declaration(NodePtr type);

  // Destructor
  virtual ~Declaration();

  // Where things in should be
  NodePtr getType() const;

  NodePtr getDeclarations() const; // Actually returns a NodeListPtr (but it's stored as a NodePtr)

  // Visualising
  virtual void PrettyPrint(std::ostream &dst, std::string indent) const override;
};

#endif
