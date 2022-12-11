BEGIN{sum=0; max_sum=0} {if (NF != 0) {sum+=$1} else {if (sum > max_sum) {max_sum=sum}; sum=0}} END{print max_sum}
