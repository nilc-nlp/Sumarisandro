<?php 
require('texts/projects/Sumarisandro/content_selection/index.php');
?>

<div class="masthead">
    <a style="text-decoration:None;" href="index.php"><h1 class="text-muted"><?php echo $title; ?></h1></a>

    <div class="bs-example bs-example-tabs">
    <ul id="myTab" class="nav nav-tabs" style="margin-bottom:5px;">
            <li rel="tooltip" title="<?php echo $tooltip_menu_ferramenta;?>" class="active"><a href="#ferramenta" data-toggle="tab"><?php echo $ferramenta; ?></a></li>
        </ul>
        <div id="myTabContent" class="tab-content">
            <div class="tab-pane fade active in" id="ferramenta">
                    <div class="well">
                        <div class="list-group-item">
				<label><input class="arquivo1" accept='text/plain' size="900000" type="file" onchange="onFileSelected(event)" style="width:300px;"/></label>
				<label><input class="arquivo2" accept='text/plain' size="900000" type="file" onchange="onFileSelected(event)" style="width:300px;"/></label>
				<textarea id="result1" style="display:none;"></textarea>
				<textarea id="result2" style="display:none;"></textarea>
                            <div class="row">
                                <div class="text-center">

                                    <br/>
                                    <div id="submit-simple">
                                        <button id="button_normalizar" class="btn btn-default" onclick="summary.call_python('button_normalizar');"><?php echo $normalize;?></button>
                                    </div>

                                </div>
                            </div>

                            <span class="label label-success"><?php echo $normalized_sentence;?></span>
                            <div contenteditable="false" id="output_sentence" class="form-control uneditable-input" rows=3 disabled style="background-color: white; color:black; height:auto; overflow:visible; min-height:70px;"></div>

                            <div class="text-center">
                                <br/>
                                <button type="button" class="btn btn-default btn-mini" onclick="summary.clean();"><?php echo $clean;?></button>
                            </div>
                        </div>
                    </div>

            </div>

        </div>
    </div>



</div>

<script src="dist/js/projects/Sumarisandro/summary/my_app.js"></script>
