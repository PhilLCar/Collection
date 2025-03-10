#include <keyval.h>

#define TYPENAME KeyVal

////////////////////////////////////////////////////////////////////////////////
KeyVal *_(Construct)(KeyValEnvironment *env)
{
  if (Pair_Construct(BASE(0))) {    
    this->env = env;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  Pair_Destruct(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
int _(Comparer)(KeyVal *reference)
{
  return this->env->comparer(BASE(0)->first, reference->base.first);
}

////////////////////////////////////////////////////////////////////////////////
int _(KeyComparer)(KeyVal *reference)
{
  return this->env->keyComparer(BASE(0)->first, reference->base.first);
}

////////////////////////////////////////////////////////////////////////////////
int _(EnvComparer)(KeyVal *reference)
{
  return ((EnvComparer)this->env->comparer)(BASE(0)->first, reference->base.first, this->env->pointer);
}

////////////////////////////////////////////////////////////////////////////////
int _(EnvKeyComparer)(KeyVal *reference)
{
  return ((EnvComparer)this->env->keyComparer)(BASE(0)->first, reference->base.first, this->env->pointer);
}

#undef TYPENAME
