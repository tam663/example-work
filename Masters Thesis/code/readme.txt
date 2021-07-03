#Basic requirments

The main package requirments are given in requirements.txt
Running experiments

To generate any of the models given in the paper, use the command: 

-For trainable skew

python main.py <name> --dataset <dset_name> --epochs <num_epochs> --loss <divergence_name> --rec-dist <distortion_metric>

-For fixed skew

python main.py <name> --dataset <dset_name> --epochs <num_epochs> --loss <divergence_name> --GJS-A <init_skew> --GJS-B <beta/lambda_weight> --rec-dist <distortion_metric> (e.g. --GJS-A sets the alpha value in skew-geometric Jensen-Shannon divergence). 

All flags and options can be found in main.py.


# Examples

The notebooks folder consists of several notebooks that recreate the experiments presented in the paper. However, the folder relies on a folder /results that we have omitted due to size limitations.


