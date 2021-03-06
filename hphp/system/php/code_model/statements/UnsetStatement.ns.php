<?php
// Copyright 2004-present Facebook. All Rights Reserved.
// This file is @generated by
// fbcode/hphp/facebook/tools/code_model/Generate.sh

namespace HH\CodeModel {

/**
 * unset(expressions);
 */
class UnsetStatement /*implements IUnsetStatement*/ {
  use Statement;
  use Node;

  private /*?Vector<IExpression>*/ $expressions;

  /**
   * unset(expressions);
   */
  public function getExpressions() /*: Vector<IExpression>*/ {
    if ($this->expressions === null) {
      $this->expressions = Vector {};
    }
    return $this->expressions;
  }
  /**
   * unset(expressions);
   */
  public function setExpressions(/*Vector<IExpression>*/ $value)
     /*: this*/ {
    $this->expressions = $value;
    return $this;
  }

  /**
   * Returns $visitor->visitUnsetStatement($this) if
   * such a method exists on $visitor.
   * Otherwise returns $visitor->visitStatement($this) if
   * such a method exists on $visitor.
   * Otherwise returns $visitor->visitNode($this) if
   */
  public function accept(/*mixed*/ $visitor) /*: mixed*/ {
    if (method_exists($visitor, "visitUnsetStatement")) {
      // UNSAFE
      return $visitor->visitUnsetStatement($this);
    } else if (method_exists($visitor, "visitStatement")) {
      // UNSAFE
      return $visitor->visitStatement($this);
    } else {
      return $visitor->visitNode($this);
    }
  }

  /**
   * Yields a list of nodes that are children of this node.
   * A node has exactly one parent, so doing a depth
   * first traversal of a node graph using getChildren will
   * traverse a spanning tree.
   */
  public function getChildren() /*: Continuation<INode>*/ {
    if ($this->expressions !== null) {
      foreach ($this->expressions as $elem) {
        yield $elem;
      }
    }
  }
}
}
