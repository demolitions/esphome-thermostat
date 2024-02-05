void setNumber(esphome::number::Number *component, int newVal) 
{
  esphome::number::NumberCall call = component->make_call();
  call.set_value(newVal);
  call.perform();
}

void setSelect(esphome::select::Select *component, const std::string &newval) {
  esphome::select::SelectCall call = component->make_call();
  call.set_option(newval);
  call.perform();
}