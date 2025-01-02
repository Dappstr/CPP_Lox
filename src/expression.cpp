#include "expression.hpp"

Value Binary_Expression::accept(Expression_Visitor &visitor) const {
	return visitor.visit(*this);
}

Value Unary_Expression::accept(Expression_Visitor &visitor) const {
	return visitor.visit(*this);
}

Value Group_Expression::accept(Expression_Visitor &visitor) const {
	return visitor.visit(*this);
}

Value Literal_Expression::accept(Expression_Visitor &visitor) const {
	return visitor.visit(*this);
}
