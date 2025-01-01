#include "expression.hpp"

void Binary_Expression::accept(Expression_Visitor &visitor) {
	visitor.visit(*this);
}

void Unary_Expression::accept(Expression_Visitor &visitor) {
	visitor.visit(*this);
}

void Group_Expression::accept(Expression_Visitor &visitor) {
	visitor.visit(*this);
}

void Literal_Expression::accept(Expression_Visitor &visitor) {
	visitor.visit(*this);
}
