#pragma once

struct TRuleAntecedent {
    int Feature = -1;
    bool Value = false;
};

struct TRule {
    TRuleAntecedent Antecedent;
    int Label = -1;
};
