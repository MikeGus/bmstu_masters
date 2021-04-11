import argparse
import csv


def get_new_dataset(dataset):
    new_dataset = {}
    for k, v in dataset.items():
        if k == 'Survived':
            new_dataset['Survived:yes'] = [1 if val == '1' else 0 for val in v]
            new_dataset['Survived:no'] = [1 if val != '1' else 0 for val in v]
        elif k == 'Pclass':
            new_dataset['Pclass:1'] = [1 if val == '1' else 0 for val in v]
            new_dataset['Pclass:2'] = [1 if val == '2' else 0 for val in v]
            new_dataset['Pclass:3'] = [1 if val == '3' else 0 for val in v]
        # elif k == 'Sex':
        #     new_dataset['Sex:male'] = [1 if val == 'male' else 0 for val in v]
        #    new_dataset['Sex:female'] = [1 if val == 'female' else 0 for val in v]
        elif k == 'Age':
            new_dataset['Age:0-10'] = [1 if (val and 0 <= float(val) < 10) else 0 for val in v]
            new_dataset['Age:10-16'] = [1 if (val and 10 <= float(val) < 16) else 0 for val in v]
            new_dataset['Age:16-25'] = [1 if (val and 16 <= float(val) < 25) else 0 for val in v]
            new_dataset['Age:25-40'] = [1 if (val and 25 <= float(val) < 40) else 0 for val in v]
            new_dataset['Age:40-55'] = [1 if (val and 40 <= float(val) < 55) else 0 for val in v]
            new_dataset['Age:55+'] = [1 if (val and 55 <= float(val)) else 0 for val in v]
        elif k == 'SibSp':
            new_dataset['SibSp:0'] = [1 if (val and 0 == int(val)) else 0 for val in v]
            new_dataset['SibSp:1'] = [1 if (val and 1 == int(val)) else 0 for val in v]
            new_dataset['SibSp:2'] = [1 if (val and 2 == int(val)) else 0 for val in v]
            new_dataset['SibSp:3+'] = [1 if (val and 3 <= int(val)) else 0 for val in v]
        elif k == 'Parch':
            new_dataset['ParCh:0'] = [1 if (val and 0 == int(val)) else 0 for val in v]
            new_dataset['ParCh:1'] = [1 if (val and 1 == int(val)) else 0 for val in v]
            new_dataset['ParCh:2'] = [1 if (val and 2 == int(val)) else 0 for val in v]
            new_dataset['ParCh:3+'] = [1 if (val and 3 <= int(val)) else 0 for val in v]
        elif k == 'Embarked':
            new_dataset['Embarked:Cherbourg'] = [1 if val == 'C' else 0 for val in v]
            new_dataset['Embarked:Queenstown'] = [1 if val == 'Q' else 0 for val in v]
            new_dataset['Embarked:Southampton'] = [1 if val == 'S' else 0 for val in v]

    return new_dataset


def print_features_and_labels(new_dataset, label, dataset_name):
    with open(dataset_name + '.out', 'w') as features_file, open(dataset_name + '.label', 'w') as labels_file:
        for k, v in new_dataset.items():
            if label.lower() in k.lower():
                labels_file.write('{{{}}} {}\n'.format(k, ' '.join([str(val) for val in v])))
            else:
                features_file.write('{{{}}} {}\n'.format(k, ' '.join([str(val) for val in v])))


def main():
    parser = argparse.ArgumentParser(description='Convert titanic features to binary.')
    parser.add_argument('-f', '--csv-file', help='csv features file', required=True)
    parser.add_argument('-l', '--label', help='which feature is considered label', required=True)
    parser.add_argument('-o', '--out-file', help='output files name', required=True)
    args = parser.parse_args()

    dataset = {}
    with open(args.csv_file) as csv_file:
        reader = csv.DictReader(csv_file)
        for row in reader:
            for k, v in row.items():
                if k not in dataset.keys():
                    dataset[k] = []
                dataset[k].append(v)
    new_dataset = get_new_dataset(dataset)
    print_features_and_labels(new_dataset, args.label, args.out_file)

if __name__ == '__main__':
    main()
