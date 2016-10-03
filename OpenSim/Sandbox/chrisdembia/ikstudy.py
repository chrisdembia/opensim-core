import opensim

exp_markers = opensim.DataSource("subject01_trial01.trc")
# Connecting multi-connector in aggregate.
exp_markers = exp_markers.getMultiOutput("exp_markers")

weights = opensim.DataSource("loadedwalking_ik_weights.csv")

ik = opensim.InverseKinematics()

ik.getMultiInput("marker_targets").connect(exp_markers)
ik.getMultiInput("marker_weights").connect(weights)

model = opensim.Model("subject01.osim")

ik.getConnector("model").connect(model)

driver.stepToEnd()




