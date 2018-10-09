DIR=$(dirname "$(readlink -f "$0")")
pushd "$PWD"
cd "$DIR"
echo "ui-test1"
maude -no-banner ui-test1.maude &> ui-test1.out
diff ui-test1.out ui-test1.expected
echo "ui-test2"
maude -no-banner ui-test2.maude &> ui-test2.out
diff ui-test2.out ui-test2.expected
echo "proofrules"
maude -no-banner proofrules.maude &> proofrules.out
diff proofrules.out proofrules.expected
echo "functions"
maude -no-banner "functions.maude" &> "functions.out"
diff "functions.out" "functions.expected"
echo "foform"
maude -no-banner "foform.maude" &> "foform.out"
diff "foform.out" "foform.expected"
echo "impsimp"
maude -no-banner impsimp.maude &> impsimp.out
diff impsimp.out impsimp.expected
popd
