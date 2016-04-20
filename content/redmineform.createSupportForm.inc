<?php
module_load_include('inc', 'redminesupport', '/system/redmineform.validation');

function redminesupport_form($form, &$form_state,$givenprojectid = null)
{
    //Redmine and GET - Settings
    $url = variable_get('redmine_api_url');
    $options = array();     
    $options['headers'] = array('Content-Type' => 'text/xml');
    $options['method'] = 'GET';
    
    if(variable_get('redmine_api_check') == '1')
    {
        $form['redmine_introduction'] = array(
            '#type' => 'markup',
            '#markup' => variable_get('redmine_api_tip'),
        );
    }
    
    //Projects
    //dsm($url.'projects.xml',$options);
    //$response = drupal_http_request($url.'projects.xml',$options);
    
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url.'projects.xml');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_VERBOSE, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    $response = curl_exec($ch);
    //dsm($response);
   
    //$response = drupal_http_request('http://redmine.gdi-de.org/projects.xml',$options);
    //dsm($response);
    //$xml = new SimpleXMLElement($response->data);
    $xml = new SimpleXMLElement($response);
    $projektliste_id =  redminesupport_get_content($xml,"//*[local-name()='project']/*[local-name()='id']");
    $projektliste_name =  redminesupport_get_content($xml,"//*[local-name()='project']/*[local-name()='name']");
    $projektliste_ = array_combine($projektliste_id,$projektliste_name);
    
    if($givenprojectid != null)
    {
        $projektliste_final = redminesupport_useFilterProjects($projektliste_,$givenprojectid);
    }
    else
    {
        $projektliste_final = redminesupport_useFilterProjects($projektliste_);
    }
    
    $form['redmine_api_project'] = array(
        '#type' => 'select',
        '#title' => t('Projekt: '),
        '#options' => $projektliste_final,
        '#required' => False,
    );
    
    //Tracker
    $trackerstatus = variable_get('redmine_api_trackerstatus');
    if($trackerstatus == '1')
    {
        //$response = drupal_http_request($url.'trackers.xml',$options);
        //$xml = new SimpleXMLElement($response->data);
        curl_setopt($ch, CURLOPT_URL, $url.'trackers.xml');
        $response = curl_exec($ch);
        //dsm($response);
        $xml = new SimpleXMLElement($response);
        $trackerliste_identifier =  redminesupport_get_content($xml,"//*[local-name()='tracker']/*[local-name()='id']");
        $trackerliste_name =  redminesupport_get_content($xml,"//*[local-name()='tracker']/*[local-name()='name']");
        $trackerliste = array_combine($trackerliste_identifier,$trackerliste_name);
        $inputProjects = variable_get('redmine_api_tracker');
        $trackerliste_final = redminesupport_useFilterTracker($trackerliste,$inputProjects);
        
        $form['redmine_api_tracker'] = array(
            '#type' => 'select',
            '#title' => t('Tracker: '),
            '#options' => $trackerliste_final,
            '#required' => TRUE,
        );
    }
    $form['redmine_ticket_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Betreff:'),
        '#size' => 83,
        '#maxlength' => 255,
        '#required' => TRUE,
    );
    $form['redmine_ticket_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Bitte geben Sie hier Ihre Meldung ein'),
        '#size' => 100,
        '#maxlength' => 10000,
        '#required' => TRUE,
    );
    if(variable_get('redmine_api_customfields') == '1')
    {
        $form['redmine_name'] = array(
            '#type' => 'textfield',
            '#title' => t('Name:'),
            '#size' => 83,
            '#maxlength' => 255,
            '#required' => TRUE,
        );
        $form['redmine_ticket_email'] = array(
            '#type' => 'textfield',
            '#title' => t('E-Mail:'),
            '#size' => 83,
            '#maxlength' => 255,
            '#required' => TRUE,
        );
        if(variable_get('redmine_api_customfields_tip_check','0') == '1')
        {
            $form['redmine_introduction'] = array(
                '#type' => 'markup',
                '#markup' => variable_get('redmine_api_customfields_tip_text'),
            );
        }
    }
    if(variable_get('redmine_api_file_check') == '1')
    {
        $form['redmine_ticket_files'] = array
        (
            '#title' => t('Anhang'),
            '#name' => 'file',
            '#type' => 'file',
            "#upload_validators"  => array("file_validate_extensions" => array("png gif jpg")),
        );
    }
    $form['submit'] = array('#type' => 'submit', '#value' => t('Ticket erstellen'));
    curl_close($ch);
    return $form;
}

function redminesupport_form_validate($form, &$form_state)
{
   
   //check email input
    if(isset($form['redmine_ticket_email']['#value']))
    {
        $mail = $form['redmine_ticket_email']['#value'];
        if (!valid_email_address($mail)) 
        {
            form_set_error('Email', t('The email address appears to be invalid.'));
        }
    }
    //check file input
//    if(!empty($_FILES['file']['name']))
//    {
//        dsm($_FILES['file']['type']);
//        variable_get();
//        if(0 != 1)
//        {
//            form_set_error('File',t('The file format is not supported'));
//        }
//    } 
}

function redminesupport_form_submit($form, &$form_state) 
{
    $json = [];
    $json['issue']['project_id'] = $form_state['values']['redmine_api_project'];
    $json['issue']['subject'] = $form_state['values']['redmine_ticket_subject'];
    
    if(isset($form_state['values']['redmine_api_tracker']))
    {
        $json['issue']['tracker_id'] = $form_state['values']['redmine_api_tracker'];
    }
    //Custom-Fields
    if(isset($form_state['values']['redmine_ticket_email']) && isset($form_state['values']['redmine_name']))
    {
        $json['issue']['custom_fields'] = array();
        $json['issue']['custom_fields'][] = array('value' => $form_state['values']['redmine_name'],'id' => '2');
        $json['issue']['custom_fields'][]= array('value' => $form_state['values']['redmine_ticket_email'], 'id' => '1');
    } 
    //Upload-File
    if(!empty($_FILES['file']['name']))
    {
        $url = variable_get('redmine_api_url').'uploads.json';
        $data = file_get_contents($_FILES['file']['tmp_name']);
        $options['method'] = 'POST';
        $options['data'] = $data;
        $options['max_redirects'] = 0;      
        $options['headers']['Authorization'] = 'Basic ' . base64_encode(variable_get('redmine_api_user') .":". (variable_get('redmine_api_password')));
        $options['headers']['Content-Type'] = 'application/octet-stream';
        $response = drupal_http_request($url, $options);
        if(variable_get('redmine_api_debug') == 1)
        {
            dsm($response);           
        }
        $filetoken = json_decode($response->data)->upload->token;   
        $json['issue']['uploads'] = array();
        $json['issue']['uploads'][] = array('token'=>$filetoken,'filename'=>$_FILES['file']['name'],'content_type'=>$_FILES['file']['type']);
    } 
    
    $json['issue']['description'] = $form_state['values']['redmine_ticket_body'];
    $json['issue']['is_private'] = variable_get('redmine_api_publish');
    //Sending Ticket
    $ch = curl_init(variable_get('redmine_api_url').'issues.json');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_VERBOSE, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");                                                                     
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($json));    
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_USERPWD, variable_get('redmine_api_user').":".variable_get('redmine_api_password'));
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(                                                                          
            'Content-Type: application/json',                                                                                
            'Content-Length: ' . strlen(json_encode($json))
        )
    );
    
    $response = curl_exec($ch);
//dsm(curl_getinfo($ch));
//dsm(curl_error($ch));
//dsm($response);   
//    //Message
    if(variable_get('redmine_api_sending_check') == '1')
    {   
        $id = json_decode($response)->issue->id;
        if(variable_get('redmine_api_sending_aslink') == 1)
        {    
            $url = variable_get('redmine_api_url').'issues/'.$id;
        }
        else
        {
            $url = '<a href="'.variable_get('redmine_api_url').'issues/'.$id.'">'.t('hier').'</a>';
        }
        $message = str_replace("{URL}",$url,variable_get('redmine_api_sending_text'));
        drupal_set_message($message, 'status');  
    }
}

function redminesupport_get_content($xml,$xpath) //Achtung Funktion kann zwei Wertetypen ausgeben String oder Array[String]
{
    $content = array();
    $inhalte = $xml->xpath($xpath);
    if(is_array($inhalte) && count($inhalte)>0)
    {
        foreach ($inhalte as $inhalt)
        {
            if($inhalt!=='' && $inhalt != null)
            {
                array_push($content,(string)$inhalt);
            }
        }
        return $content;
    }
    else
    {
        return null;
    }
}

function redminesupport_useFilterProjects($allProjects,$givenprojectid = null)
{   
    $inputProjects = variable_get('redmine_api_project');
    //wenn keine Projekte angegeben sind werden alle zur Auswahl angeboten
    if(null == $inputProjects && $givenprojectid == null)
    {
        return $allProjects;
    }
    else
    {
        $_array = array();
        if(strpos($inputProjects,','))
        {
            $_array = explode(',',$inputProjects);
        }
        else
        {
            $_array[0] = $inputProjects;
        }
        //bei 0 werden nur die angegebenen Projekte angezeigt; bei 1 werden diese rausgefiltert
        if(variable_get('redmine_api_which') == '0')
        {
            if($givenprojectid == null)
            {
                foreach ($allProjects as $project => $value) 
                {
                    if(!in_array($project,$_array))
                    {
                        unset($allProjects[$project]);
                    }
                }
            }
            else
            {
                if(in_array($givenprojectid,$_array))
                {  
                    $_array2 = array();
                    $_array2[0] = $givenprojectid;
                    foreach ($allProjects as $project => $value) 
                    {
                        if(!in_array($project,$_array2))
                        {
                            unset($allProjects[$project]);
                        }
                    }
                }
                else
                {
                    foreach ($allProjects as $project => $value) 
                    {
                        drupal_set_message('kein offiziell unterstütztes Redmine-Projekt',$type = 'warning',$repeat=FALSE);
                        unset($allProjects[$project]); 
                    }
                }
            }
        }
        else
        {
            if($givenprojectid == null)
            {
                foreach ($_array as $project) 
                {
                    if(array_key_exists($project,$allProjects))
                    {
                        unset($allProjects[$project]);
                    }
                }
            }
            else
            {
                if(!in_array($givenprojectid,$_array))
                {  
                    foreach ($allProjects as $project => $value) 
                    {
                        if($project != $givenprojectid)
                        {
                            unset($allProjects[$project]); 
                        }    
                    }
                }
                else
                {
                    foreach ($allProjects as $project => $value) 
                    {
                        drupal_set_message('kein offiziell unterstütztes Redmine-Projekt',$type = 'warning',$repeat=FALSE);
                        unset($allProjects[$project]); 
                    }
                }
            }
        }
        return $allProjects;
    }  
}

function redminesupport_useFilterTracker($allProjects,$inputProjects)
{   
    
    $_array = array();
    if(strpos($inputProjects,','))
    {
        $_array = explode(',',$inputProjects);
    }
    else
    {
        $_array[0] = $inputProjects;
    }
    //bei 0 werden nur die angegebenen Projekte angezeigt; bei 1 werden diese rausgefiltert
    if(variable_get('redmine_api_tracker_switcher') == '0')
    {
        foreach ($allProjects as $project => $value) 
        {
            if(!in_array($project,$_array))
            {
                unset($allProjects[$project]);
            }
        }
    }
    else
    {
        foreach ($_array as $project) 
        {
            if(array_key_exists($project,$allProjects))
            {
                unset($allProjects[$project]);
            }
        }
    }
    return $allProjects;  
}